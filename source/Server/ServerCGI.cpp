/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:43:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/15 18:33:37 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "Logger.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <signal.h>
#include <cstdlib>
#include <cstring>

bool Server::isCGI(std::string path) {
	std::string extension = getFileExtension(path);
	for (std::map<std::string, std::string>::const_iterator it = this->config.cgi_rules.begin(); it != this->config.cgi_rules.end(); it++) {
		if (it->first == extension)
			return (true);
	}
	return (false);
}

std::string Server::getCGI(std::string path) {
	std::string extension = getFileExtension(path);
	for (std::map<std::string, std::string>::const_iterator it = this->config.cgi_rules.begin(); it != this->config.cgi_rules.end(); it++) {
		if (it->first == extension)
			return (it->second);
	}
	throw std::runtime_error("Unable to find CGI");
}

static std::string initWorkingDirectory(std::string path) {
	int i = path.length();
	while (i >= 0) {
		if (path[i] == '/')
			break;
		i--;
	}
	std::string file = std::string(path, i + 1);
	std::string folder = std::string(path.begin(), path.begin() + i);
	if (!folder.empty()) {
		if (chdir(folder.c_str()) == -1) {
			logger << ERROR << "chdir failed: " << strerror(errno) << ENDL;
		}
	}
	return (file);
}

Response Server::execCGI(Client &client, std::string path) {
	Response &response = client.response;
	CGI &cgi = response.cgi;
	std::string cgiExec = this->getCGI(path);

	logger << DEBUG << "Trying to execute CGI: " << cgiExec << " " << path << ENDL;
	
	// Check if the file exist
	if (access(path.c_str(), F_OK) != 0)
		response = this->getErrorResponse(RESPONSE_NOT_FOUND);
	// Check if the file can be executed
	else if (access(path.c_str(), R_OK) != 0 || access(cgiExec.c_str(), X_OK) != 0)
		response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
	// File can be executed
	else {
		cgi.path = path;

		int fds[2];
		if (pipe(fds) == -1) {
			logger << CRITICAL << "Unable to open pipe: " << std::strerror(errno) << ENDL;
			response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
			return (response);
		}
		
		int pid = fork();
		if (pid == -1) {
			logger << CRITICAL << "Unable to create fork: " << std::strerror(errno) << ENDL;
			response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
			return (response);
		}
	
		if (pid == 0) {
			std::string file = initWorkingDirectory(path);

			// Exec args
			const char *argv[3];
			argv[0] = cgiExec.c_str();
			argv[1] = file.c_str();
			argv[2] = __null;
			
			// env
			char * const *env = this->getCGIEnv(client, file);

			dup2(fds[1], 1);
			close(fds[0]);
			if (execve(cgiExec.c_str(), (char * const *)argv, env) == -1) {
				logger << ERROR << "Failed to execute CGI '" << cgiExec << "': " << std::strerror(errno) << ENDL;
			}
			// Free env
			for (unsigned int i = 0; env[i]; i++) 
				delete [] env[i];
			delete [] env;
			
			close(fds[1]);
			close(1);
			throw Server::CGIError();
		}
		close(fds[1]);
		cgi.is_running = 1;
		cgi.timeout = time(__null);
		cgi.pid = pid;
		cgi.fd = fds[0];
	}
	return (response);
}

// Return true if eof or error occured 
bool Server::readCGI(Client &client) {
	CGI &cgi = client.response.cgi;

	char buffer[65536];

	int byteReads = read(cgi.fd, buffer, sizeof(buffer));
	if (byteReads == -1) {
		client.response = this->getErrorResponse(500);
		sendResponse(client);
		return (true);
	} else if (byteReads == 0) {
		cgi.eof = 1;
		logger << DEBUG << client << " > CGI Received an end-of-file" << ENDL;
		return (true);
	}
	else {
		logger << DEBUG << client << " > CGI Received " << byteReads << " byte(s)" << ENDL;
		cgi.output += std::string(buffer, byteReads);
	}
	return (false);
}

static Response parseCGI(std::string &output) {
	Response response;

	std::string::iterator it = output.begin();
	// headers
	while (
		it != output.end() &&
		!expectedToken(output, it, "\n") &&
		!expectedToken(output, it, "\r\n")
	) {
		std::string key;
		while (it != output.end() && !expectedToken(output, it, ": ")) {
			key += *it;
			it++;
		}

		std::string value;
		while (
			it != output.end() &&
			!(expectedToken(output, it, "\n") || expectedToken(output, it, "\r\n"))
		) {
			value += *it;
			it++;
		}
		response.headers[key] = value;
	}
	// content
	response.content = std::string(it, output.end());
	
	if (response.headers.has("Content-Type")) {
		response.content_type = response.headers["Content-Type"];
		response.headers.erase("Content-Type");
	}
	if (response.headers.has("Status")) {
		response.status_code = std::atoi(response.headers["Status"].c_str());
		response.headers.erase("Status");
	} else {
		response.status_code = 200;
	}
	if (response.headers.has("Location")) {
		response.status_code = RESPONSE_MOVED_PERMANENTLY;
	}

	return (response);
}

// send cgi output to client
// return true if connection != keep-alive (say to serverManager to remove client socket)
bool Server::onCGIOutput(Client &client) {
	if (!client.response.cgi.output.empty()) {
		client.response = parseCGI(client.response.cgi.output);
	} else {
		logger << WARNING << client << " > CGI output is empty! (" << client.response.cgi.path << ")" << ENDL;
		client.response = this->getErrorResponse(500);
	}
	
	std::string connectionType = client.request.headers["connection"];
	this->sendResponse(client);
	if (connectionType != "keep-alive")
		return (true);
	return (false);
}

void Server::onCGITimeout(Client &client) {
	this->stopCGI(client);
	
	logger << WARNING << client << " > CGI timed out (" << client.response.cgi.path << ")" << ENDL;
	client.response = this->getErrorResponse(504);
	this->sendResponse(client);
}

void Server::stopCGI(Client &client) {
	CGI &cgi = client.response.cgi;

	if (cgi.fd != -1) {
		close(cgi.fd);
		cgi.fd = -1;
	}
	if (kill(cgi.pid, SIGKILL) == -1) {
		logger << CRITICAL << "kill failed: " << std::strerror(errno) << ENDL;
	} else {
		logger << DEBUG << client << " > CGI successfully stopped!" << ENDL;
	}
}

char * const *Server::getCGIEnv(Client &client, std::string file) {
	std::map<std::string, std::string> variables;
	
	// define all env variable
	variables["CONTENT_LENGTH"] = toString(client.request.headers.getContentLength());
	variables["CONTENT_TYPE"] = client.request.headers["content-type"];
	variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	variables["QUERY_STRING"] = client.request.raw_parameters;
	variables["REMOTE_ADDR"] = client.address;
	variables["REMOTE_HOST"] = "";
	variables["REQUEST_METHOD"] = client.request.raw_method;
	variables["SCRIPT_NAME"] = file;
	variables["SERVER_NAME"] = client.request.headers.has("host") ? client.request.headers["host"] : getHostname();
	variables["SERVER_PORT"] = this->getPort();
	variables["SERVER_PROTOCOL"] = "HTTP/1.1";
	variables["SERVER_SOFTWARE"] = "webserv/0.1";
	// Request headers
	for (Headers::const_iterator header = client.request.headers.begin(); header != client.request.headers.end(); header++) {
		std::string key = header->first;
		strToUpper(key);
		variables["HTTP_" + key] = header->second;
	}
	
	unsigned int length = std::distance(variables.begin(), variables.end());
	char **env = new char *[length + 1];
	unsigned int i = 0;
	for (std::map<std::string, std::string>::iterator variable = variables.begin(); variable != variables.end(); variable++) {
		std::string rawVariable = variable->first + "=" + variable->second;
		env[i] = new char[rawVariable.length() + 1];
		std::strcpy(env[i], rawVariable.c_str());
		i++;
	}
	env[length] = __null;
	return (env);
}