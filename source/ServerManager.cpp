/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:51:27 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 21:42:52 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Server.hpp"

#include <exception>
#include <sys/select.h>
#include <vector>
#include <signal.h>
#include <sys/socket.h>

int isRunning = 1;

void	signalHandler(int sig) {
	(void)sig;
	isRunning = 0;
}

ServerManager::ServerManager(void) {}

void ServerManager::addServer(Server server) {
	_servers.push_back(server);
}

void ServerManager::run(void) {
	signal(SIGINT, signalHandler);
	logger << INFO << "Starting server(s)..." << ENDL;

	fd_set read_sockets, write_sockets;
	FD_ZERO(&_master_socket);

	// Initialize Socket
	logger << DEBUG << "Initializing servers socket..." << ENDL;
	for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
		int serverSocket = server->initSocket();
		FD_SET(serverSocket, &_master_socket);
	}
	logger << DEBUG << "Server(s) socket initialized!" << ENDL;
	logger << INFO << "Server(s) successfully started! (Press CTRL+C to quit)" << ENDL;
	for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
		logger << INFO << "'" << server->getConfig().name << "'" " is running on " << "http://" << server->getConfig().listen << ENDL;
	}
	while (isRunning) {
		read_sockets = _master_socket;
		write_sockets = _master_socket;
		if (select(FD_SETSIZE, &read_sockets, &write_sockets, __null, __null) == -1) {
			if (isRunning)
				logger << ERROR << "Select failed" << ENDL;
			break;
		}
		for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
			int serverSocket = server->getSocket();

			// server socket
			if (FD_ISSET(serverSocket, &read_sockets)) {
				// NEW CONNECTION
				try {
					Client client = server->acceptClient();
					FD_SET(client.getSocket(), &_master_socket);	
					logger << DEBUG << client << " > New client" << ENDL;
				} catch (std::exception &e) {
					logger << ERROR << e.what() << ENDL;
				}
			}
			
			std::vector<Client> &clientSockets = server->getClients();
			std::vector<Client> clientToRemove;

			// clients socket
			for (std::vector<Client>::iterator client = clientSockets.begin(); client != clientSockets.end(); ++client) {
				Request &request = client->getRequest();

				// Read segments
				if (FD_ISSET(client->getSocket(), &read_sockets)) {
					try {
						// First Segment (check http request syntax)
						if (request.getSegmentCount() == 0)
							server->receiveFirstSegment(*client);
						// Other (complete the content)
						else
							server->receiveOtherSegment(*client);
					} catch (std::exception &e) {
						logger << INFO << *client << " > " << e.what() << ENDL;
						clientToRemove.push_back(*client);
					}
				}

				// If the number of segment is above 0 &&
				// we can write into socket &&
				// the request is completely read
				if (
					request.getSegmentCount() > 0 &&
					FD_ISSET(client->getSocket(), &write_sockets) &&
					server->isEndOfSegment(*client)
				) {
					if (server->onRequest(*client))
						clientToRemove.push_back(*client);
				}
				
				// Check segment timeout
				else if (
					request.getSegmentTimeout() != -1 && 
					time(__null) > request.getSegmentTimeout() + 5
				) {
					// If we can write, send a 408 Request Timeout
					// Close client connection
					if (FD_ISSET(client->getSocket(), &write_sockets))
						server->onSegmentTimeout(*client);
					clientToRemove.push_back(*client);
					continue;
				}
				
				// Keep Alive timeout
				if (time(__null) > client->getClientTimeout() + server->getConfig().keepalive_timeout) {
					server->onKeepAliveTimeout(*client);
					clientToRemove.push_back(*client);
				}
			}

			// clients to remove
			for (std::vector<Client>::iterator client = clientToRemove.begin(); client != clientToRemove.end(); ++client) {
				server->closeClient(*client);
				FD_CLR(client->getSocket(), &_master_socket);
			}
		}
	}
	logger << INFO << "Closing server(s)..." << ENDL;
	this->closeServers();
	logger << INFO << "Server(s) closed!" << ENDL;
}

void ServerManager::closeServers(void) {
	for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
		std::vector<Client> clients = server->getClients();
		for (std::vector<Client>::iterator client = clients.begin(); client != clients.end(); client++) {
			server->closeClient(*client);
		}
		server->closeSocket();
	}
}