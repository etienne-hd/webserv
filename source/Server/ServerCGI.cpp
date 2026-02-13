/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:43:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 22:54:55 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include "Logger.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>
#include <unistd.h>

bool Server::isCGI(std::string path) {
	std::string extension = getFileExtension(path);
	for (std::map<std::string, std::string>::const_iterator it = _config.cgi_rules.begin(); it != _config.cgi_rules.end(); it++) {
		if (it->first == extension)
			return (true);
	}
	return (false);
}

std::string Server::getCGI(std::string path) {
	std::string extension = getFileExtension(path);
	for (std::map<std::string, std::string>::const_iterator it = _config.cgi_rules.begin(); it != _config.cgi_rules.end(); it++) {
		if (it->first == extension)
			return (it->second);
	}
	throw std::runtime_error("Unable to find CGI");
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
			// Exec args
			const char *argv[3];
			argv[0] = cgiExec.c_str();
			argv[1] = path.c_str();
			argv[2] = __null;
			
			dup2(fds[1], 1);
			close(fds[0]);
			if (execve(cgiExec.c_str(), (char * const *)argv, __null) == -1) {
				logger << ERROR << "Failed to execute CGI '" << cgiExec << "': " << std::strerror(errno) << ENDL;
			}
			close(fds[1]);
		}
		close(fds[1]);
		cgi.is_running = 1;
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
		while (it != output.end() && !expectedToken(output, it, "\n")) {
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

	return (response);
}

void Server::onCGIOutput(Client &client) {
	CGI &cgi = client.response.cgi;
	client.response = parseCGI(cgi.output);
	sendResponse(client);
}