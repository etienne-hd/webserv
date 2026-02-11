/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:19:14 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 20:11:49 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "Method.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <dirent.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <cstring>

Server::Server(const Config config): _config(config) {
	_socket = -1;
}

Server::~Server(void) {}

int Server::initSocket(void) {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	int option = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	sockaddr_in socketAddress = getSocketAddress(_config.listen);
	if (bind(_socket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) == -1) {
		throw std::runtime_error("Unable to bind address");
	}
	listen(_socket, 25);
	return (_socket);
}

void Server::closeSocket(void) {
	if (_socket == -1) {
		logger << WARNING << "Unable to close socket: socket is not init." << ENDL;
		return;
	}
	if (close(_socket) == -1)
		logger << CRITICAL << "Syscall close return -1" << ENDL;
	else
		logger << DEBUG << "Server socket successfully closed." << ENDL;
}

Client Server::acceptClient(void) {
	sockaddr_in addr;
	unsigned int addrSize = sizeof(addr);
	int clientSocket = accept(_socket, (sockaddr *)&addr, &addrSize);
	if (clientSocket == -1) {
		logger << ERROR << "accept failed" << ENDL;
		throw std::runtime_error("accept failed");
	}
	Client client(clientSocket, addr.sin_addr.s_addr, addr.sin_port);
	_clients.push_back(client);
	return (client);
}

void Server::receiveSegment(Client &client) {
	// Re-set segment timeout
	if (client.getTotalSegment() == 0)
		client.getSegmentTimeout() = time(__null);
	char *buffer = new char[65536];
	
	ssize_t byteReads = recv(client.getSocket(), buffer, 65536, 0);
	if (byteReads == 0) {
		delete[] buffer;
		throw Server::ClientDisconnected();
	}
	if (byteReads == -1) {
		delete[] buffer;
		throw std::runtime_error(std::strerror(errno));
	}
	
	std::string rawRequest = std::string(buffer, byteReads);
	client.getRawRequest() += rawRequest;
	client.getTotalSegment()++;
	logger << DEBUG << client << " > Received segment #" << client.getTotalSegment() << " " << byteReads << " byte(s)" << ENDL;
	
	delete[] buffer;
}

// Check if the server need to read more segment from the client.
// return true when all segment is read or when the request has error in it
bool Server::isEndOfSegment(Client &client) {
	std::string currentSegment = client.getRawRequest();
	if (currentSegment.empty())
		return (false);

	Request request;
	try {
		request = Request();
	} catch (std::exception &e) {
		return (true); // 400 Bad Request
	}
	
	unsigned long contentLength = request.getHeaders().getContentLength();
	if (contentLength > this->_config.max_body_size)
		return (true); // 413 Content Too Large

	return (request.getContent().length() >= contentLength);
}

// onRequest is called when an incomming full request is ready
// It return a boolean, if true the socket must be closed
bool Server::onRequest(Client &client) {
	Request request;
	Response response;

	client.getTotalRequest()++;
	client.getClientTimeout() = time(__null);
	std::string rawContent = client.getRawRequest();
	client.resetSegment();
	try {
		request = Request(rawContent);
	} catch (Request::BadRequest &e) {
		logger << DEBUG << client << " -> " << "Did a bad request (Invalid HTTP Request)." << ENDL;
		response = this->getErrorResponse(400);
		sendResponse(client, response);
		return (true);
	}

	unsigned int contentLength = request.getHeaders().getContentLength();
	if (request.getHTTPVersion() != "HTTP/1.1") {
		logger << DEBUG << "Inavlid HTTP Version: " << request.getHTTPVersion() << ENDL;
		response = this->getErrorResponse(505); // 505 HTTP Version Not Supported
	} else if (!this->isAllowedMethod(request.getMethod()))
		response = this->getErrorResponse(501); // Not Implemented
	else if (request.getContent().length() > _config.max_body_size || contentLength > _config.max_body_size)
		response = this->getErrorResponse(413); // Content Too Large
	else if (request.getContent().length() != contentLength)
		response = this->getErrorResponse(400);
	else if (request.getMethod() == GET)
		response = this->getResponse(request); // Get file / folder
	else if (request.getMethod() == POST) {}
		// create file
	else if (request.getMethod() == DELETE) {}
		// delete file
	else
		response = this->getErrorResponse(400);

	logger << INFO << client << " (" << client.getTotalRequest() << ")" << " -> " << response.getStatusCode() << " " << request.getRawMethod() << " " << request.getUri() << ENDL;
	sendResponse(client, response);
	
	if (request.getHeaders()["connection"] == "keep-alive")
		return (false);
	else
		return (true);
}

void Server::onSegmentTimeout(Client &client) {
	Response response;

	logger << INFO << client << " -> Segment timed out, connection closed." << ENDL; 
	response = getErrorResponse(408);
	sendResponse(client, response);
}

void Server::onKeepAliveTimeout(Client &client) {
	logger << INFO << client << " -> Keep Alive timed out, connection closed" << ENDL; 
}

void Server::closeClient(Client &client) {
	close(client.getSocket());
	std::vector<Client>::iterator currentClient = _clients.begin();
	for (; currentClient != _clients.end(); currentClient++) {
		if (client == *currentClient) {
			_clients.erase(currentClient);
			currentClient--;
			break;
		}
	}
	if (currentClient == _clients.end())
		logger << ERROR << "Unable to remove " << client << " from vector." << ENDL;
	else
		logger << DEBUG << client << " -> Client socket successfully closed." << ENDL;
}