/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:28:41 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 10:07:29 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "Server.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <stdexcept>
#include <sys/socket.h>

bool Server::isRedirection(std::string uri) const {
	for (std::map<std::string, std::string>::const_iterator redirection = _config.redirections.begin(); redirection != _config.redirections.end(); ++redirection) {
		if (isSamePath(uri, redirection->first))
			return (1);
	}
	return (0);
}

std::string Server::getRedirection(std::string uri) const {
	for (std::map<std::string, std::string>::const_iterator redirection = _config.redirections.begin(); redirection != _config.redirections.end(); ++redirection) {
		if (isSamePath(uri, redirection->first))
			return (redirection->second);
	}
	throw std::runtime_error("Unable to find redirection.");
}

std::string Server::getRawRequest(int clientSocket) const {
	// 8192 -> Method + Uri + HTTP Version + Headers
	char *buffer = new char[_config.max_body_size + 65536 + 1];
	
	int code = recv(clientSocket, buffer, _config.max_body_size + 65536, 0);
	if (code == 0) {
		delete[] buffer;
		throw Server::ClientDisconnected();
	}
	if (code == -1) {
		delete[] buffer;
		throw std::runtime_error(std::strerror(errno));
	}
	buffer[code] = 0;
	
	std::string rawRequest = buffer;
	delete[] buffer;

	return (rawRequest);
}

static void removeDuplicateSlash(std::string &s) {
	for (std::string::iterator it = s.begin(); it != s.end(); it++) {
		if (*it == '/' && *(it + 1) == '/')
		{
			s.erase(it);
			it--;
		}
	}
}

std::string Server::locationResolver(std::string uri) const {
	std::map<std::string, std::string> locationsMap = _config.locations;
	std::map<std::string, std::string>::const_iterator locations = locationsMap.begin();
	std::string closest;

	logger << DEBUG << "Location Resolver: " << uri << " -> ";
	removeDuplicateSlash(uri);
	if (uri == "/")
		uri = _config.document_index;

	while (locations != locationsMap.end()) {
		if (std::strncmp(uri.c_str(), locations->first.c_str(), locations->first.length()) == 0) {
			if (closest.length() < locations->first.length()) {
				closest = locations->first;
			}
		}
		++locations;
	}
	if (closest.empty())
		return ("." + uri);
	std::string subUri = uri.substr(closest.length(), uri.length());
	std::string path = locationsMap[closest] + (subUri[0] != '/' ? "/" : "") + subUri;
	removeDuplicateSlash(path);
	logger << path << ENDL;
	return path;
}

bool Server::isAllowedMethod(Method method) {
	std::vector<Method>::const_iterator it = _config.allowed_methods.begin();
	for (; it != _config.allowed_methods.end(); it++) {
		if (*it == method)
			break;
	}
	return (it != _config.allowed_methods.end());
}