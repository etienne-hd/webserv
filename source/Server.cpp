/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:19:14 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 01:04:23 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

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

std::ostream &operator<<(std::ostream &stream, Server *server) {
	stream << "[" << server->getConfig().name << "] ";
	return (stream);
}