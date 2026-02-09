/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:19:14 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 04:43:56 by ehode            ###   ########.fr       */
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
	logger << DEBUG << "Server Constructor called (" << config.name << ")" << ENDL;
	_socket = -1;
}

Server::~Server(void) {
	logger << DEBUG << "Server Destructor called (" << _config.name << ")" << ENDL;
	if (_socket != -1)
	{
		logger << INFO << this << "Closing server socket..." << ENDL;
		close(_socket);
	}
}

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

int Server::onNewClient(void) {
	int clientSocket = accept(_socket, __null, __null);
	if (clientSocket == -1) {
		logger << ERROR << "accept failed" << ENDL;
		return (-1);
	}
	_client_sockets.push_back(clientSocket);
	return (clientSocket);
}

void Server::onRequest(int clientSocket) {
	std::string rawRequest = this->getRawRequest(clientSocket);
	Request request(rawRequest);
	logger << INFO << request.getRawMethod() << " " << request.getUri() << ENDL;
}

void Server::closeClient(int clientSocket) {
	close(clientSocket);
	for (std::vector<int>::iterator it = _client_sockets.begin(); it != _client_sockets.end(); it++) {
		if (clientSocket == *it) {
			_client_sockets.erase(it);
			break;
		}
	}
}

std::ostream &operator<<(std::ostream &stream, Server *server) {
	stream << "[" << server->getConfig().name << "] ";
	return (stream);
}