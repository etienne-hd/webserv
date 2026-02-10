/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:30:31 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 15:48:25 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "Client.hpp"
# include "Response.hpp"

#include <exception>
# include <sys/select.h>
# include <vector>

#include "Request.hpp"

class Server {
	private:
		const Config _config;
		int	_socket;
		std::vector<Client> _clients;

		std::string getRawRequest(int clientSoscket) const;
		void sendResponse(Client &client, Response &response);
		Response getResponse(Request &request);
		Response getErrorResponse(int status_code);
		bool isRedirection(std::string uri) const;
		std::string getRedirection(std::string uri) const;
		std::string locationResolver(std::string &uri) const;

	public:
		Server(const Config config);
		~Server(void);

		int initSocket(void);
		int getSocket(void) { return _socket; }
		std::vector<Client> &getClientSockets(void) { return _clients; }
		void closeClient(Client &client);

		const Config getConfig(void) const { return _config; }

		// event
		Client onNewClient(void);
		void onRequest(Client &client);

		class ClientDisconnected: public std::exception {
			virtual const char *what(void) const throw() { return "Client disconnected."; }
		};
		class NotFound: public std::exception {
			virtual const char *what(void) const throw() { return "404: Not found"; }
		};
		class RequestEntityTooLarge: public std::exception {
			virtual const char *what(void) const throw() { return "413: Request Entity Too Large"; }
		};
		//class ClientDisconnected: public std::exception {
		//	virtual const char *what(void) const throw() { return "Client disconnected."; }
		//};
};

#endif