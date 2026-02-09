/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:51:27 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 01:07:23 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "Logger.hpp"
#include "Server.hpp"

#include <vector>

ServerManager::ServerManager(std::vector<Server *> servers): _servers(servers) {}

void ServerManager::run(void) {
	logger << INFO << "Starting server(s)..." << ENDL;

	// Initialize Socket
	for (std::vector<Server *>::iterator it = _servers.begin(); it != _servers.end(); it++) {
		Server *server = *it;
		logger << DEBUG << server << "Initializing server socket..." << ENDL;
		int socket = server->initSocket();
		logger << DEBUG << server << "Successfully initialized socket: " << socket << ENDL;
	}
}