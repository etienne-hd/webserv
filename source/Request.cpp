/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 02:45:56 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 04:43:38 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Method.hpp"
#include "Logger.hpp"

#include <cstring>
#include <iostream>
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

Request::Request(std::string rawRequest) {
	RequestToken currentTokenType = METHOD;
	std::string currentToken;
	std::string::iterator it = rawRequest.begin();
	while (it != rawRequest.end()) {
		if (currentTokenType == METHOD && expectedToken(rawRequest, it, " ")) {
			_raw_method = currentToken;
			_method = getMethodFromString(currentToken);
			currentTokenType = URI;
			std::cout << currentToken << std::endl;
			currentToken.clear();
		} else if (currentTokenType == URI && expectedToken(rawRequest, it, " ")) {
			_uri = currentToken;
			currentTokenType = HTTP_VERSION;
			std::cout << currentToken << std::endl;
			currentToken.clear();
		} else if (currentTokenType == HTTP_VERSION && expectedToken(rawRequest, it, "\r\n")) {
			_http_version = currentToken;
			currentTokenType = HEADERS;
			std::cout << currentToken << std::endl;
			currentToken.clear();
		} else if (currentTokenType == HEADERS) {
			while (it != rawRequest.end() && !expectedToken(rawRequest, it, "\r\n")) {
				std::string key;
				while (it != rawRequest.end() && !expectedToken(rawRequest, it, ": ")) {
					key += *it;
					it++;
				}
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
	_content = currentToken;
}