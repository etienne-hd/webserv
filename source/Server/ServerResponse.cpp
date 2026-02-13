/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 15:23:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 14:50:37 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <cstring>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

void Server::sendResponse(Client &client, Response &response) {
	logger << INFO << client << " (" << client.getTotalRequest() << ")" << " -> " << response.getStatusCode() << " " << client.getRequest().getRawMethod() << " " << client.getRequest().getRawUri() << ENDL;
	client.resetRequest();

	std::string rawResponse = response.build();
	if (send(client.getSocket(), rawResponse.c_str(), rawResponse.length(), 0) == -1) {
		throw std::runtime_error("Unable to send response");
	}
}

Response Server::getResponse(Request &request) {
	Response response;

	std::string uri = request.getUri();

	if (this->isRedirection(uri)) {
		std::map<std::string, std::string> redirections = this->_config.redirections;
		response.getStatusCode() = RESPONSE_MOVED_PERMANENTLY;
		response.getHeaders()["Location"] = this->getRedirection(uri);
		return (response);
	}
	std::string path = locationResolver(uri);

	DIR *dir = getDirectory(path);
	// if path is a directory
	if (dir) {
		logger << DEBUG << path << " is a directory" << ENDL;
		// if directory listing is enabled
		if (_config.directory_listing_enabled) {
			std::string content = getFileListing(dir, request.getUri());
			response.getContentType() = "text/html";
			response.getContent() = content;
		// else show file on directory
		} else {
			path = this->locationResolver(_config.file_on_directory);
			response = this->getFileResponse(path);
		}
		closedir(dir);
	// else path is a file
	} else {
		// Check if its a CGI
		if (false) {

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

	response.getStatusCode() = status_code;
	std::map<int, std::string> error_pages = _config.error_pages;
	response.getContentType() = "text/html";
	if (error_pages.find(status_code) == error_pages.end())
		response.getContent() = getDefaultErrorContent(status_code);
	else {
		std::string path = this->locationResolver(error_pages[status_code]);
		response = this->getFileResponse(path);
	}
	return (response);
}

Response Server::getFileResponse(const std::string path) {
	Response response;

	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1) {
		response = this->getErrorResponse(RESPONSE_NOT_FOUND);
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
			logger << ERROR << "Read error: " << std::strerror(errno) << ENDL;
			response = this->getErrorResponse(RESPONSE_INTERNAL_SERVER_ERROR);
		} else {
			response.getStatusCode() = RESPONSE_OK;
			response.getContent() = content;
			response.setContentTypeByPath(path);
		}
		close(fd);
	}
	return (response);
}

Response Server::getCreateFileResponse(Request &request) {
	Response response;
	
	if (_config.file_upload_enabled) {
		std::string path = locationResolver(_config.file_upload_directory) + request.getUri();
		logger << DEBUG << "Trying to create file at " << path << ENDL;
		int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
		if (fd != -1) {
			write(fd, request.getContent().c_str(), request.getContent().length());
			response.getStatusCode() = RESPONSE_CREATED;
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

	if (_config.file_upload_enabled) {
		std::string path = locationResolver(_config.file_upload_directory) + request.getUri();
		logger << DEBUG << "Trying to remove file at " << path << ENDL;
		if (access(path.c_str(), F_OK) == 0) {
			if (std::remove(path.c_str()) != -1) {
				response.getStatusCode() = RESPONSE_OK;
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