/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:30:31 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 04:48:54 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"

# include <iostream>
# include <sys/select.h>
# include <vector>

class Server {
	private:
		const Config _config;
		int	_socket;
		std::vector<int> _client_sockets;

		std::string getRawRequest(int clientSocket);
	public:
		Server(const Config config);
		~Server(void);

		int initSocket(void);
		int getSocket(void) { return _socket; }
		std::vector<int> &getClientSockets(void) { return _client_sockets; }
		void closeClient(int clientSocket);

		const Config getConfig(void) const { return _config; }

		// event
		int onNewClient(void);
		void onRequest(int clientSocket);

		class ClientDisconnected: public std::exception {
			virtual const char *what(void) const throw() { return "Client disconnected."; }
		};
};

std::ostream &operator<<(std::ostream &stream, Server *server);

#endif