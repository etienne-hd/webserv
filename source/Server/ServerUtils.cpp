/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:28:41 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 23:27:58 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "Server.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

bool Server::isRedirection(std::string uri) const {
	for (std::map<std::string, std::string>::const_iterator redirection = this->config.redirections.begin(); redirection != this->config.redirections.end(); ++redirection) {
		if (isSamePath(uri, redirection->first))
			return (1);
	}
	return (0);
}

std::string Server::getRedirection(std::string uri) const {
	for (std::map<std::string, std::string>::const_iterator redirection = this->config.redirections.begin(); redirection != this->config.redirections.end(); ++redirection) {
		if (isSamePath(uri, redirection->first))
			return (redirection->second);
	}
	throw std::runtime_error("Unable to find redirection.");
}

std::string Server::locationResolver(std::string uri) const {
	std::map<std::string, std::string> locationsMap = this->config.locations;
	std::map<std::string, std::string>::const_iterator locations = locationsMap.begin();
	std::string closest;

	logger << DEBUG << "Location Resolver: " << uri << " -> ";
	removeDuplicateSlash(uri);
	if (uri == "/")
		uri = this->config.document_index;

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

bool Server::isAllowedMethod(Method method) const {
	std::vector<Method>::const_iterator it = this->config.allowed_methods.begin();
	for (; it != this->config.allowed_methods.end(); it++) {
		if (*it == method)
			break;
	}
	return (it != this->config.allowed_methods.end());
}