/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:51:27 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 19:46:52 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "Logger.hpp"
#include "Server.hpp"

#include <exception>
#include <sys/select.h>
#include <vector>

ServerManager::ServerManager(void) {}

void ServerManager::addServer(Server server) {
	_servers.push_back(server);
}

void ServerManager::run(void) {
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
	while (1) {
		read_sockets = _master_socket;
		write_sockets = _master_socket;
		if (select(FD_SETSIZE, &read_sockets, &write_sockets, __null, __null) == -1) {
			logger << ERROR << "Select failed" << ENDL;
			break;
		}
		for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
			int serverSocket = server->getSocket();

			// server socket
			if (FD_ISSET(serverSocket, &read_sockets)) {
				// NEW CONNECTION
				try {
					logger << INFO << "New connection!" << ENDL;
					Client client = server->onNewClient();
					logger << DEBUG << "New client: " << client << ENDL;
					FD_SET(client.getSocket(), &_master_socket);	
				} catch (std::exception &e) {
					logger << ERROR << e.what() << ENDL;
				}
			}
			
			std::vector<Client> clientSockets = server->getClientSockets();

			// client sockets
			for (std::vector<Client>::iterator client = clientSockets.begin(); client != clientSockets.end(); client++) {
				if (FD_ISSET(client->getSocket(), &read_sockets) && FD_ISSET(client->getSocket(), &write_sockets)) {
					// NEW INCOMING DATA
					logger << DEBUG << "New Data!" << ENDL;
					try {
						server->onRequest(*client);
					} catch (std::exception &e) {
						// If an error occured: close client
						// closeClient can probably make some error due to delete of a Client
						server->closeClient(*client);
						FD_CLR(client->getSocket(), &_master_socket);
						logger << INFO << e.what() << ENDL;
					}
				}
			}
		}
	}
}