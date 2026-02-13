/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:19:14 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 23:45:20 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Server.hpp"
#include "Headers.hpp"
#include "Method.hpp"
#include "Request.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include "Logger.hpp"

#include <dirent.h>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

Server::Server(const Config config): config(config) {
	_socket = -1;
}

Server::~Server(void) {}

int Server::initSocket(void) {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	int option = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	sockaddr_in socketAddress = getSocketAddress(this->config.listen);
	if (bind(_socket, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) == -1) {
		this->closeSocket();
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
	else {
		logger << DEBUG << "Server socket successfully closed." << ENDL;
		_socket = -1;
	}
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

// onRequest is called when an incomming full request is ready
// It return a boolean, if true the socket must be closed
bool Server::onRequest(Client &client) {
	Request &request = client.request;
	Response &response = client.response;
	CGI &cgi = response.cgi;

	request.segment_timeout = -1;
	client.client_timeout = -1;

	if (request.pre_response_status_code != -1) {
		response = this->getErrorResponse(request.pre_response_status_code);
		sendResponse(client);
		return (true);
	}

	// These checks can only be performed once all segments are connected.
	if (request.content.length() != request.headers.getContentLength()) {
		response = this->getErrorResponse(RESPONSE_BAD_REQUEST);
		sendResponse(client);
		return (true);
	}
	if (request.content.length() > this->config.max_body_size) {
		response = this->getErrorResponse(RESPONSE_CONTENT_TOO_LARGE);
		sendResponse(client);
		return (true);
	}
	
	if (request.method == GET)
		response = this->getResponse(client); // Get file / folder
	else if (request.method == POST)
		response = this->getCreateFileResponse(request);
	else if (request.method == DELETE)
		response = this->getDeleteFileResponse(request);
	else
		response = this->getErrorResponse(RESPONSE_BAD_REQUEST);

	if (cgi.is_running == false)
		sendResponse(client);
	if (request.headers["connection"] == "keep-alive" || cgi.is_running == true)
		return (false);
	else
		return (true);
}

void Server::onSegmentTimeout(Client &client) {

	client.response = getErrorResponse(408);
	sendResponse(client);
	logger << INFO << client << " -> Segment timed out, connection closed." << ENDL; 
}

void Server::onKeepAliveTimeout(Client &client) {
	logger << INFO << client << " -> Keep Alive timed out, connection closed" << ENDL; 
}

void Server::closeClient(Client &client) {
	close(client.socket);
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