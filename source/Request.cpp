/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 02:45:56 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 19:22:04 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Method.hpp"
#include "utils.hpp"

#include <cstring>
#include <exception>
#include <string>
#include <sys/socket.h>

static bool expectedToken(std::string &s, std::string::iterator &it, std::string expectedToken) {
	int distance = std::distance(s.begin(), it);
	if (std::strncmp(std::string(s.begin() + distance, s.end()).c_str(), expectedToken.c_str(), expectedToken.length()) == 0)
	{
		it += expectedToken.length();
		return (1);
	}
	return (0);
}

void Request::initParameters(void) {
	size_t pos = _raw_uri.find("?");
	if (pos == std::string::npos)
		return; // No parameters in request
	_raw_parameters = std::string(_raw_uri, pos + 1);

	std::string::iterator it = _raw_parameters.begin();
	while (it != _raw_parameters.end()) {
		std::string key;
		while (it != _raw_parameters.end() && !expectedToken(_raw_parameters, it, "=")) {
			key += *it;
			it++;
		}

		std::string value;
		while (it != _raw_parameters.end() && !expectedToken(_raw_parameters, it, "&")) {
			value += *it;
			it++;
		}

		_parameters[key] = value;
	}
}

void Request::initUri(void) {
	size_t pos = _raw_uri.find("?");
	if (pos == std::string::npos) {
		_uri = _raw_uri;
		return; // No parameters in request
	}
	_uri = std::string(_raw_uri, 0, pos);
}

Request::Request(void) {}

Request::Request(std::string rawRequest) {
	RequestToken currentTokenType = METHOD;
	std::string currentToken;
	std::string::iterator it = rawRequest.begin();
	while (it != rawRequest.end()) {
		if (currentTokenType == METHOD && expectedToken(rawRequest, it, " ")) {
			_raw_method = currentToken;
			try {
				_method = getMethodFromString(currentToken);
			} catch (std::exception &e) {
				throw BadRequest();
			}
			currentTokenType = URI;
			currentToken.clear();
		} else if (currentTokenType == URI && expectedToken(rawRequest, it, " ")) {
			_raw_uri = currentToken;
			currentTokenType = HTTP_VERSION;
			currentToken.clear();
		} else if (currentTokenType == HTTP_VERSION && expectedToken(rawRequest, it, "\r\n")) {
			_http_version = currentToken;
			currentTokenType = HEADERS;
			currentToken.clear();
		} else if (currentTokenType == HEADERS) {
			while (it != rawRequest.end() && !expectedToken(rawRequest, it, "\r\n")) {
				std::string key;
				while (it != rawRequest.end() && !expectedToken(rawRequest, it, ": ")) {
					key += *it;
					it++;
				}
				strToLower(key);
				std::string value;
				while (it != rawRequest.end() && !expectedToken(rawRequest, it, "\r\n")) {
					value += *it;
					it++;
				}
				_headers[key] = value;
			}
			currentTokenType = CONTENT;
		} else {
			currentToken += *it;
			it++;
		}
	}
	if (
		currentTokenType != CONTENT ||
		rawRequest.find("\r\n\r\n") == std::string::npos
	)
		throw Request::BadRequest();
	_content = currentToken;
	this->initParameters();
	this->initUri();
}