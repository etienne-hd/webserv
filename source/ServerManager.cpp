/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:51:27 by ehode             #+#    #+#             */
/*   Updated: 2026/02/15 18:28:00 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "CGI.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Server.hpp"

#include <cstring>
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

	fd_set read_fds, write_fds;
	FD_ZERO(&_master_fds);

	// Initialize Socket
	logger << DEBUG << "Initializing servers socket..." << ENDL;
	try {
		for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
			int serverSocket = server->initSocket();
			FD_SET(serverSocket, &_master_fds);
		}
	} catch (std::exception &e) {
		logger << ERROR << "Initialization failed: " << e.what() << ENDL;
		for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
			server->closeSocket();
		}
		return ;
	}
	
	logger << DEBUG << "Server(s) socket initialized!" << ENDL;
	logger << INFO << "Server(s) successfully started! (Press CTRL+C to quit)" << ENDL;
	for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
		logger << INFO << "'" << server->config.name << "'" " is running on " << "http://" << server->config.listen << ENDL;
	}
	
	// main loop
	while (isRunning) {
		read_fds = _master_fds;
		write_fds = _master_fds;
		if (select(FD_SETSIZE, &read_fds, &write_fds, __null, __null) == -1) {
			if (isRunning)
				logger << CRITICAL << "select failed: " << std::strerror(errno) << ENDL;
			break;
		}
		for (std::vector<Server>::iterator server = _servers.begin(); server != _servers.end(); server++) {
			int serverSocket = server->getSocket();

			// server socket
			if (FD_ISSET(serverSocket, &read_fds)) {
				// NEW CONNECTION
				try {
					Client client = server->acceptClient();
					FD_SET(client.socket, &_master_fds);	
					logger << DEBUG << client << " > New client" << ENDL;
				} catch (std::exception &e) {
					logger << ERROR << e.what() << ENDL;
				}
			}
			
			std::vector<Client> &clientSockets = server->getClients();
			std::vector<Client> clientToRemove;

			// clients socket
			for (std::vector<Client>::iterator client = clientSockets.begin(); client != clientSockets.end(); ++client) {
				Request &request = client->request;
				CGI &cgi = client->response.cgi;

				if (cgi.is_running) {
					int cgiFd = cgi.fd; // tmp variable of the read pipe (if read failed, request is reset including fd)
					if (FD_ISSET(cgi.fd, &read_fds)) {
						if (server->readCGI(*client)) {
							server->stopCGI(*client);
							FD_CLR(cgiFd, &_master_fds);
						}
						if (cgi.eof) {
							if (server->onCGIOutput(*client))
								clientToRemove.push_back(*client);
						}
					} else if (time(__null) >= cgi.timeout + server->config.cgi_timeout) {
						server->onCGITimeout(*client);
						FD_CLR(cgiFd, &_master_fds);
					}
					continue;
				}

				// Read client request
				if (
					FD_ISSET(client->socket, &read_fds) &&
					request.pre_response_status_code == -1
				){
					try {
						// First Segment (check http request syntax)
						if (request.segment_count == 0)
							server->receiveFirstSegment(*client);
						// Other (complete the content)
						else
							server->receiveOtherSegment(*client);
					} catch (std::exception &e) {
						logger << INFO << *client << " > " << e.what() << ENDL;
						clientToRemove.push_back(*client);
					}
				}

				// Request is ready
				if (
					request.segment_count > 0 &&
					FD_ISSET(client->socket, &write_fds) &&
					server->isEndOfSegment(*client)
				) {
					try {
						if (server->onRequest(*client))
							clientToRemove.push_back(*client); // keep-alive not present or malformed request
	
						// If CGI is running add read pipe to master fd
						if (cgi.is_running) {
							FD_SET(cgi.fd, &_master_fds);
						}
					} catch (Server::CGIError &e) {
						this->closeServers();
						return ;
					} catch (std::exception &e) {
						logger << ERROR << e.what() << ENDL;
						clientToRemove.push_back(*client);
					}
				}
				
				// Check segment timeout
				if (
					request.segment_timeout != -1 && 
					time(__null) >= request.segment_timeout + 5
				) {
					// If we can write, send a 408 Request Timeout
					// Close client connection
					if (FD_ISSET(client->socket, &write_fds))
						server->onSegmentTimeout(*client);
					clientToRemove.push_back(*client);
				}
				
				// Keep Alive timeout
				else if (
					client->client_timeout != -1 &&
					time(__null) >= client->client_timeout + server->config.keepalive_timeout
				) {
					server->onKeepAliveTimeout(*client);
					clientToRemove.push_back(*client);
				}
			}

			// clients to remove
			for (std::vector<Client>::iterator client = clientToRemove.begin(); client != clientToRemove.end(); ++client) {
				if (client->response.cgi.is_running)
					server->stopCGI(*client);
				server->closeClient(*client);
				FD_CLR(client->socket, &_master_fds);
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
			if (client->response.cgi.is_running)
				server->stopCGI(*client);
			server->closeClient(*client);
		}
		server->closeSocket();
	}
}