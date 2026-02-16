/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 15:23:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/16 13:44:25 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <cstring>
#include <unistd.h>
#include <fcntl.h>

void Server::sendResponse(Client &client) {
	Response &response = client.response;

	logger << INFO << client << " (" << client.total_request << ")" << " -> " << response.status_code << " " << client.request.raw_method << " " << client.request.raw_uri << ENDL;
	std::string rawResponse = response.build();
	client.resetIO();
	if (send(client.socket, rawResponse.c_str(), rawResponse.length(), 0) == -1) {
		logger << CRITICAL << "Unable to send response: " << std::strerror(errno) << ENDL;
		throw std::runtime_error("Unable to send response");
	}
	client.client_timeout = time(__null);
}

Response Server::getResponse(Client &client) {
	Response response;
	Request request = client.request;

	std::string uri = request.uri;

	if (this->isRedirection(uri)) {
		response.status_code = RESPONSE_MOVED_PERMANENTLY;
		response.headers["Location"] = this->getRedirection(uri);
		return (response);
	}
	std::string path = locationResolver(uri);

	DIR *dir = getDirectory(path);
	// if path is a directory
	if (dir) {
		logger << DEBUG << path << " is a directory" << ENDL;
		// if directory listing is enabled
		if (this->config.directory_listing_enabled) {
			std::string content = getFileListing(dir, request.uri);
			response.content_type = "text/html";
			response.content = content;
		// else show file on directory
		} else {
			path = this->locationResolver(this->config.file_on_directory);
			response = this->getFileResponse(path);
		}
		closedir(dir);
	// else path is a file
	} else {
		// Check if its a CGI
		if (this->config.cgi_enabled && this->isCGI(path)) {
			response = this->execCGI(client, path);
		} else {
			response = this->getFileResponse(path);
		}
	}
	return (response);
}

static std::string getDefaultErrorContent(int status_code) {
	std::stringstream ss;

	ss << "<html><body style='width:100%; height:  100%; display:flex; flex-direction:column; justify-items:start; align-items:center;'>";
	ss << "<h1>" << status_code << " " << Response::getStatusCodeText(status_code) << "</h1>";
	ss << "<hr style='width: 100%'/>";
	ss << "<p>webserv</p>";
	ss << "</body></html>";
	return (ss.str());
}

Response Server::getErrorResponse(int status_code) {
	Response response;

	response.status_code = status_code;
	std::map<int, std::string> error_pages = this->config.error_pages;
	response.content_type = "text/html";
	if (error_pages.find(status_code) != error_pages.end()) {
		std::string path = this->locationResolver(error_pages[status_code]);
		if (access(path.c_str(), R_OK) == 0)
			response = this->getFileResponse(path);
		else {
			logger << WARNING << "Provided error " << status_code << " page '" << path << "' cannot be read!" << ENDL;
			response.content = getDefaultErrorContent(status_code);
		}
	} else
		response.content = getDefaultErrorContent(status_code);
	return (response);
}

Response Server::getFileResponse(const std::string path) {
	Response response;

	if (access(path.c_str(), F_OK) != 0)
		response = this->getErrorResponse(RESPONSE_NOT_FOUND);
	else if (access(path.c_str(), R_OK) != 0)
		response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
	else {
		int fd = open(path.c_str(), O_RDONLY);
		if (fd == -1) {
			response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
		} else {
			std::string content;
			char buffer[65536];
			int byteReads;
			
			while (1) {
				byteReads = read(fd, buffer, sizeof(buffer));
				if (byteReads <= 0)
					break;
				content += std::string(buffer, byteReads);
			}
			if (byteReads == -1) {
				logger << CRITICAL << "read error: " << std::strerror(errno) << ENDL;
				response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
			} else {
				response.status_code = RESPONSE_OK;
				response.content = content;
				response.setContentTypeByPath(path);
			}
			close(fd);
		}
	}
	return (response);
}

Response Server::getCreateFileResponse(Request &request) {
	Response response;
	
	if (this->config.file_upload_enabled) {
		std::string path = locationResolver(this->config.file_upload_directory) + request.uri;
		logger << DEBUG << "Trying to create file at " << path << ENDL;
		int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
		if (fd != -1) {
			write(fd, request.content.c_str(), request.content.length());
			response.status_code = RESPONSE_CREATED;
			close(fd);
		} else {
			response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
		}
	} else {
		response = this->getErrorResponse(RESPONSE_SERVICE_UNAVAILABLE);
	}

	return (response);
}

Response Server::getDeleteFileResponse(Request &request) {
	Response response;

	if (this->config.file_upload_enabled) {
		std::string path = locationResolver(this->config.file_upload_directory) + request.uri;
		logger << DEBUG << "Trying to remove file at " << path << ENDL;
		if (access(path.c_str(), F_OK) == 0) {
			if (std::remove(path.c_str()) != -1) {
				response.status_code = RESPONSE_OK;
			} else {
				logger << DEBUG << "Unable to remove file at " << path << " reason: " << std::strerror(errno) << ENDL;
				response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
			}
		} else // No such file or directory
			response = this->getErrorResponse(RESPONSE_NOT_FOUND);
	} else
		response = this->getErrorResponse(RESPONSE_SERVICE_UNAVAILABLE);

	return (response);
}

std::string Server::getFileListing(DIR *dir, const std::string &currentUri) {
	struct dirent *entry = readdir(dir);
	std::stringstream ss;
	
	ss << "<html><body>";
	ss << "<h1>Index of " << currentUri << "</h1>";
	ss << "<hr style='width: 100%;'/>";
	ss << "<ul>";
	while (entry != NULL) {
		ss << "<li>"
		<< "<a href='" << currentUri + (currentUri[currentUri.length() - 1] != '/' ? "/" : "")
		+ entry->d_name << "'>"
		<< entry->d_name << (entry->d_type == DT_DIR ? "/" : "")
		<< "</a>"
		<< "</li>";
		entry = readdir(dir);
	}
	ss << "<ul/>";
	ss << "</html></body>";
	return ss.str();
}