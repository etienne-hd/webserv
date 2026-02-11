/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 15:23:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 19:51:44 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include "utils.hpp"

#include <fstream>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
#include <dirent.h>

void Server::sendResponse(Client &client, Response &response) {
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
		response.getStatusCode() = 301;
		response.getHeaders()["Location"] = this->getRedirection(uri);
		return (response);
	}
	std::string path = locationResolver(uri);

	DIR *dir = getDirectory(path);
	if (dir) {
		logger << DEBUG << path << " is a directory" << ENDL;
		if (_config.directory_listing_enabled) {
			std::string content = getFileListing(dir, request.getUri());
			response.getContentType() = "text/html";
			response.getContent() = content;
		} else {
			path = this->locationResolver(_config.file_on_directory);
			std::fstream file(path.c_str());
			if (!file.is_open()) {
				response = getErrorResponse(404);
			} else {
				std::getline(file, response.getContent(), '\0');
				response.setContentTypeByPath(path);
			}
		}
		closedir(dir);
	} else {
		// Check if its a CGI
		std::fstream file(path.c_str());
		if (!file.is_open()) {
			response = getErrorResponse(404);
		} else {
			std::getline(file, response.getContent(), '\0');
			response.setContentTypeByPath(path);
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
		std::ifstream file(path.c_str());
		if (file.is_open()) {
			std::getline(file, response.getContent(), '\0');
			response.setContentTypeByPath(path);
		} else
			response.getContent() = getDefaultErrorContent(status_code);
	}
	return (response);
}

std::string Server::getFileListing(DIR *dir, const std::string &currentUri) {
	struct dirent *entry = readdir(dir);
	std::stringstream ss;

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
	return ss.str();
}
