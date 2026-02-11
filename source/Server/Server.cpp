/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:19:14 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 10:32:13 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <dirent.h>
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

void Server::onRequest(Client &client) {
	Request request;
	Response response;

	std::string rawRequest = this->getRawRequest(client.getSocket());
	logger << DEBUG << "raw request: \n" << rawRequest << ENDL;
	try {
		request = Request(rawRequest);
	} catch (Request::BadRequest &e) {
		logger << DEBUG << client << " -> " << "Did a bad request (Invalid HTTP Request)." << ENDL;
		response = this->getErrorResponse(400);
		sendResponse(client, response);
		return;
	}

	if (request.getHTTPVersion() != "HTTP/1.1")
		response = this->getErrorResponse(505);
	else if (!this->isAllowedMethod(request.getMethod()))
		response = this->getErrorResponse(501);
	else if (request.getContent().length() > _config.max_body_size)
		response = this->getErrorResponse(413);
	else if (request.getMethod() == GET)
		response = this->getResponse(request);
	else if (request.getMethod() == POST) {
		// create file
	}
	logger << INFO << client << " -> " << response.getStatusCode() << " " << request.getRawMethod() << " " << request.getUri() << ENDL;
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
	else
		logger << DEBUG << client << " -> Client socket successfully closed." << ENDL;
}