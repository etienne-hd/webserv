/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:30:31 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 01:54:33 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "Response.hpp"
# include "Request.hpp"

# include <iostream>
# include <vector>

class Server {
	private:
		const Config _config;
		int	_socket;
		std::vector<int> _clientSocket;
	
	public:
		Server(const Config config);
		~Server(void);

		int initSocket(void);
		int getSocket(void) { return _socket; }
		std::vector<int> &getClientSocket(void) { return _clientSocket; }
		int closeSocket(void);

		const Config getConfig(void) const { return _config; }

		// event
		int onNewClient(void);
		Response onRequest(Request &request);
};

std::ostream &operator<<(std::ostream &stream, Server *server);

#endif