/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:30:31 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 18:43:51 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <dirent.h>

# include "Config.hpp"
# include "Client.hpp"
# include "Response.hpp"

# include <exception>
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
		bool isAllowedMethod(Method method);
		static std::string getFileListing(DIR *dir, const std::string &currentUri);

	public:
		Server(const Config config);
		~Server(void);

		
		int initSocket(void);
		int getSocket(void) { return _socket; }
		void closeSocket(void);
		
		std::vector<Client> &getClients(void) { return _clients; }
		void closeClient(Client &client);

		const Config getConfig(void) const { return _config; }

		// event
		Client onNewClient(void);
		void onRequest(Client &client);

		// exception
		class ClientDisconnected: public std::exception {
			virtual const char *what(void) const throw() { return "Client disconnected."; }
		};
};

#endif