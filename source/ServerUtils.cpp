/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:28:41 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 10:49:45 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "Server.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <sys/socket.h>

#include "Logger.hpp"

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
	char *buffer = new char[_config.max_body_size + 8192 + 2];
	
	int code = recv(clientSocket, buffer, _config.max_body_size + 8192 + 1, 0);
	if (code == 0) {
		delete[] buffer;
		throw Server::ClientDisconnected();
	}
	if (code == -1) {
		delete[] buffer;
		throw std::runtime_error("recv error");
	}
	buffer[code] = 0;
	
	std::string rawRequest = buffer;
	delete[] buffer;
	
	if (rawRequest.length() > _config.max_body_size + 8192 + 1)
		throw Server::RequestEntityTooLarge();

	return (rawRequest);
}

std::string Server::locationResolver(const std::string &uri) const {
	std::map<std::string, std::string> locationsMap = _config.locations;
	std::map<std::string, std::string>::const_iterator locations = locationsMap.begin();
	std::string closest;

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
	const std::string subUri = uri.substr(closest.length(), uri.length());
	std::string path = locationsMap[closest] + (subUri[0] != '/' ? "/" : "") + subUri;
	return path;
}