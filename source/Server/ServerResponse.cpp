/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 15:23:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 19:12:14 by ehode            ###   ########.fr       */
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
			// Directory Listing
		} else { 
			std::string file_on_directory = _config.file_on_directory;
			path = this->locationResolver(file_on_directory);
			std::fstream file(path.c_str());
			if (!file.is_open()) {
				response = getErrorResponse(404);
			} else {
				std::getline(file, response.getContent(), '\0');
				response.setContentTypeByPath(path);
			}
		}
			// send file on directory
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

	ss << "<html><body>";
	ss << "<h1>" << status_code << " " << Response::getStatusCodeText(status_code) << "</h1>";
	ss << "</body></html>";
	return (ss.str());
}

Response Server::getErrorResponse(int status_code) {
	Response response;

	response.getStatusCode() = status_code;
	std::map<int, std::string> error_pages = _config.error_pages;
	if (error_pages.find(status_code) == error_pages.end()) {
		response.getContent() = getDefaultErrorContent(status_code);
	} else {
		std::string path = this->locationResolver(error_pages[status_code]);
		std::ifstream file(path.c_str());
		if (file.is_open()) {
			std::getline(file, response.getContent(), '\0');
		} else {
			response.getContent() = getDefaultErrorContent(status_code);
		}
	}
	return (response);
}