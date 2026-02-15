/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 19:55:28 by ehode             #+#    #+#             */
/*   Updated: 2026/02/15 20:14:46 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Server.hpp"
#include "Logger.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <cstring>

std::string Server::receiveSegment(Client &client) {
	Request &request = client.request;
	// Reset Timeout
	request.segment_timeout = time(__null);
	client.client_timeout = time(__null);
	
	char buffer[262144];
	
	ssize_t byteReads = recv(client.socket, buffer, sizeof(buffer), 0);
	if (byteReads == 0)
		throw Server::ClientDisconnected();
	else if (byteReads == -1)
		throw std::runtime_error(std::strerror(errno));
	
	std::string rawSegment = std::string(buffer, byteReads);
	request.segment_count++;
	logger << DEBUG << client << " > Received segment #" << request.segment_count << " of " << byteReads << " byte(s)" << ENDL;
	return (rawSegment);
}

// Check if the server need to read more segment from the client.
// return true when all segment is read
bool Server::isEndOfSegment(Client &client) {
	Request &request = client.request;

	if (request.pre_response_status_code != -1)
		return (true);

	unsigned long contentLength = request.headers.getContentLength();
	return (request.content.length() >= contentLength);
}

static void initParameters(Request &request) {
	size_t pos = request.raw_uri.find("?");
	if (pos == std::string::npos)
		return; // No parameters in request
	request.raw_parameters = std::string(request.raw_uri, pos + 1);

	std::string::iterator it = request.raw_parameters.begin();
	while (it != request.raw_parameters.end()) {
		std::string key;
		while (it != request.raw_parameters.end() && !expectedToken(request.raw_parameters, it, "=")) {
			key += *it;
			it++;
		}

		std::string value;
		while (it != request.raw_parameters.end() && !expectedToken(request.raw_parameters, it, "&")) {
			value += *it;
			it++;
		}

		request.parameters[key] = value;
	}
}

static void initUri(Request &request) {
	std::string &uri = request.uri;
	std::string &rawUri = request.raw_uri;
	size_t pos = rawUri.find("?");
	if (pos == std::string::npos) {
		uri = request.raw_uri;
	} else {
		uri = std::string(request.raw_uri, 0, pos);
	}
	removeDuplicateSlash(uri);
	if (uri.length() > 1 && *(uri.end() - 1) == '/')
		uri.erase(uri.end() - 1);
}

static void parseRequest(Request &request, std::string rawSegment) {
	RequestToken currentTokenType = METHOD;
	std::string currentToken;
	std::string::iterator it = rawSegment.begin();
	while (it != rawSegment.end()) {
		if (currentTokenType == METHOD && expectedToken(rawSegment, it, " ")) {
			request.raw_method = currentToken;
			try {
				request.method = getMethodFromString(currentToken);
			} catch (std::exception &e) {
				throw Server::BadRequest();
			}
			currentTokenType = URI;
			currentToken.clear();
		} else if (currentTokenType == URI && expectedToken(rawSegment, it, " ")) {
			request.raw_uri = currentToken;
			currentTokenType = HTTP_VERSION;
			currentToken.clear();
		} else if (currentTokenType == HTTP_VERSION && expectedToken(rawSegment, it, "\r\n")) {
			request.http_version = currentToken;
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
				request.headers[key] = value;
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

	request.content = currentToken;
	initParameters(request);
	initUri(request);
}

// Receive the first segment, it check the headers and verify that the request is correctly formatted
void Server::receiveFirstSegment(Client &client) {
	Request &request = client.request;
	client.total_request++;
	
	std::string rawSegment = this->receiveSegment(client);
	
	try {
		parseRequest(request, rawSegment);
	} catch (std::exception &e) {
		request.pre_response_status_code = RESPONSE_BAD_REQUEST;
		return ;
	}

	if (!this->isAllowedMethod(request.method))
		request.pre_response_status_code = RESPONSE_NOT_IMPLEMENTED;
	else if (request.http_version != "HTTP/1.1")
		request.pre_response_status_code = RESPONSE_HTTP_VERSION_NOT_SUPPORTED;
	else if (request.headers.getContentLength() > this->config.max_body_size)
		request.pre_response_status_code = RESPONSE_CONTENT_TOO_LARGE;
	else if (!isValidUri(request.uri))
		request.pre_response_status_code = RESPONSE_BAD_REQUEST;
}

void Server::receiveOtherSegment(Client &client) {
	Request &request = client.request;
	std::string rawSegment = this->receiveSegment(client);

	request.content += rawSegment;
}

void Server::onSegmentTimeout(Client &client) {
	client.response = getErrorResponse(408);
	sendResponse(client);
	logger << INFO << client << " -> Segment timed out, connection closed." << ENDL; 
}