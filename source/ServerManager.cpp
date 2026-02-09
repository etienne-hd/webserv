/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:51:27 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 04:49:02 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Server.hpp"

#include <exception>
#include <sys/select.h>
#include <vector>

ServerManager::ServerManager(std::vector<Server *> servers): _servers(servers) {}

void ServerManager::run(void) {
	logger << INFO << "Starting server(s)..." << ENDL;

	fd_set read_sockets, write_sockets;
	FD_ZERO(&_master_socket);

	// Initialize Socket
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		Server *server = *it;
		logger << DEBUG << server << "Initializing server socket..." << ENDL;
		int serverSocket = server->initSocket();
		FD_SET(serverSocket, &_master_socket);
		logger << DEBUG << server << "Successfully initialized socket: " << serverSocket << ENDL;
	}

	while (1) {
		read_sockets = _master_socket;
		write_sockets = _master_socket;
		if (select(FD_SETSIZE, &read_sockets, &write_sockets, __null, __null) == -1) {
			logger << ERROR << "Select failed" << ENDL;
			break;
		}
		for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
			Server *server = *it;
			int serverSocket = server->getSocket();

			// server socket
			if (FD_ISSET(serverSocket, &read_sockets)) {
				// NEW CONNECTION
				logger << INFO << "New connection!" << ENDL;
				int clientSocket = server->onNewClient();
				if (clientSocket != -1) {
					logger << DEBUG << "New client " << clientSocket << ENDL;
					FD_SET(clientSocket, &_master_socket);
				}
			}
			
			std::vector<int> clientSockets = server->getClientSockets();

			// client sockets
			for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); it++) {
				int clientSocket = *it;
				if (FD_ISSET(clientSocket, &read_sockets) && FD_ISSET(clientSocket, &write_sockets)) {
					// NEW INCOMING DATA
					logger << DEBUG << "New Data!" << ENDL;
					try {
						server->onRequest(clientSocket);
					} catch (Server::ClientDisconnected &e) {
						server->closeClient(clientSocket);
						FD_CLR(clientSocket, &_master_socket);
						logger << INFO << "Client " << clientSocket << " disconnected" << ENDL;
					} catch (std::exception &e) {
						logger << ERROR << e.what() << ENDL;
					}
					
				}
			}
		}
	}
}