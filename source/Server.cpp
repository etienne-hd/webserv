/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:19:14 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 10:50:06 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

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

Client Server::onNewClient(void) {
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
	}

	// Resolve path
	// Check if its a directory
	// Check if its a CGI

	
	//response.getContent() = "Hello from C++!";
	//response.getStatusCode() = 200;
	//response.getCookies()["Hello,"] = "World!";

	return (response);
}

void Server::onRequest(Client &client) {
	client.getTotalRequest()++;
	std::string rawRequest = this->getRawRequest(client.getSocket());
	Request request(rawRequest);
	if (request.getContent().length() > _config.max_body_size)
		throw Server::RequestEntityTooLarge();
	
	logger << INFO << client << " > " << request.getRawMethod() << " " << request.getUri() << ENDL;
	
	Response response = this->getResponse(request);
	sendResponse(client, response);
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
}