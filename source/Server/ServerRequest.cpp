/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 19:55:28 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 16:48:30 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <exception>
#include <sys/socket.h>
#include <cstring>

std::string Server::receiveSegment(Client &client) {
	Request &request = client.getRequest();
	// Reset Timeout
	request.getSegmentTimeout() = time(__null);
	client.getClientTimeout() = time(__null);
	
	char buffer[262144];
	
	ssize_t byteReads = recv(client.getSocket(), buffer, sizeof(buffer), 0);
	if (byteReads == 0)
		throw Server::ClientDisconnected();
	else if (byteReads == -1)
		throw std::runtime_error(std::strerror(errno));
	
	std::string rawSegment = std::string(buffer, byteReads);
	request.getSegmentCount()++;
	logger << DEBUG << client << " > Received segment #" << request.getSegmentCount() << " of " << byteReads << " byte(s)" << ENDL;
	return (rawSegment);
}

// Check if the server need to read more segment from the client.
// return true when all segment is read
bool Server::isEndOfSegment(Client &client) {
	Request &request = client.getRequest();

	if (request.getPreResponseStatusCode() != -1)
		return (true);

	unsigned long contentLength = request.getHeaders().getContentLength();
	return (request.getContent().length() >= contentLength);
}

static bool expectedToken(std::string &s, std::string::iterator &it, std::string expectedToken) {
	int distance = std::distance(s.begin(), it);
	if (std::strncmp(std::string(s.begin() + distance, s.end()).c_str(), expectedToken.c_str(), expectedToken.length()) == 0)
	{
		it += expectedToken.length();
		return (1);
	}
	return (0);
}

static void initParameters(Request &request) {
	size_t pos = request.getRawUri().find("?");
	if (pos == std::string::npos)
		return; // No parameters in request
	std::string raw_parameters = std::string(request.getRawUri(), pos + 1);
	request.getRawParameters() = raw_parameters;

	std::string::iterator it = raw_parameters.begin();
	while (it != raw_parameters.end()) {
		std::string key;
		while (it != raw_parameters.end() && !expectedToken(raw_parameters, it, "=")) {
			key += *it;
			it++;
		}

		std::string value;
		while (it != raw_parameters.end() && !expectedToken(raw_parameters, it, "&")) {
			value += *it;
			it++;
		}

		request.getParameters()[key] = value;
	}
}

static void initUri(Request &request) {
	std::string &uri = request.getUri();
	std::string &rawUri = request.getRawUri();
	size_t pos = rawUri.find("?");
	if (pos == std::string::npos) {
		uri = request.getRawUri();
	} else {
		uri = std::string(request.getRawUri(), 0, pos);
	}
	removeDuplicateSlash(uri);
	if (uri.length() > 1 && *(uri.end() - 1) == '/')
		uri.erase(uri.end() - 1);
	logger << CRITICAL << uri << ENDL;
}

static void parseRequest(Request &request, std::string rawSegment) {
	RequestToken currentTokenType = METHOD;
	std::string currentToken;
	std::string::iterator it = rawSegment.begin();
	while (it != rawSegment.end()) {
		if (currentTokenType == METHOD && expectedToken(rawSegment, it, " ")) {
			request.getRawMethod() = currentToken;
			try {
				request.getMethod() = getMethodFromString(currentToken);
			} catch (std::exception &e) {
				throw Server::BadRequest();
			}
			currentTokenType = URI;
			currentToken.clear();
		} else if (currentTokenType == URI && expectedToken(rawSegment, it, " ")) {
			request.getRawUri() = currentToken;
			currentTokenType = HTTP_VERSION;
			currentToken.clear();
		} else if (currentTokenType == HTTP_VERSION && expectedToken(rawSegment, it, "\r\n")) {
			request.getHTTPVersion() = currentToken;
			currentTokenType = HEADERS;
			currentToken.clear();
		} else if (currentTokenType == HEADERS) {
			while (it != rawSegment.end() && !expectedToken(rawSegment, it, "\r\n")) {
				
				std::string key;
				while (it != rawSegment.end() && !expectedToken(rawSegment, it, ": ")) {
					key += *it;
					it++;
				}
				strToLower(key);
				
				std::string value;
				while (it != rawSegment.end() && !expectedToken(rawSegment, it, "\r\n")) {
					value += *it;
					it++;
				}
				request.getHeaders()[key] = value;
			}
			currentTokenType = CONTENT;
		} else {
			currentToken += *it;
			it++;
		}
	}

	if (
		currentTokenType != CONTENT ||
		rawSegment.find("\r\n\r\n") == std::string::npos
	)
		throw Server::BadRequest();

	request.getContent() = currentToken;
	initParameters(request);
	initUri(request);
}

static bool isValidUri(std::string uri) {
	if (uri[0] != '/')
		return (false);

	std::stringstream ss(uri);
	std::string part;
	while (getline(ss, part, '/')) {
		if (part == "..")
			return (false);
	}
	return (true);
}

// Receive the first segment, it check the headers and verify that the request is correctly formatted
void Server::receiveFirstSegment(Client &client) {
	Request &request = client.getRequest();
	client.getTotalRequest()++;
	
	std::string rawSegment = this->receiveSegment(client);
	
	try {
		parseRequest(request, rawSegment);
	} catch (std::exception &e) {
		request.getPreResponseStatusCode() = RESPONSE_BAD_REQUEST;
		return ;
	}

	if (!this->isAllowedMethod(request.getMethod()))
		request.getPreResponseStatusCode() = RESPONSE_NOT_IMPLEMENTED;
	else if (request.getHTTPVersion() != "HTTP/1.1")
		request.getPreResponseStatusCode() = RESPONSE_HTTP_VERSION_NOT_SUPPORTED;
	else if (request.getHeaders().getContentLength() > _config.max_body_size)
		request.getPreResponseStatusCode() = RESPONSE_CONTENT_TOO_LARGE;
	else if (!isValidUri(request.getUri()))
		request.getPreResponseStatusCode() = RESPONSE_BAD_REQUEST;
}

void Server::receiveOtherSegment(Client &client) {
	Request &request = client.getRequest();
	std::string rawSegment = this->receiveSegment(client);

	request.getContent() += rawSegment;
}