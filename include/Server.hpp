/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:30:31 by ehode             #+#    #+#             */
/*   Updated: 2026/02/14 19:39:01 by ehode            ###   ########.fr       */
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
		int	_socket;
		std::vector<Client> _clients;


		std::string getRawRequest(int clientSocket) const;
		
		// Response
		void sendResponse(Client &client);
		Response getResponse(Client &client);
		Response getErrorResponse(int status_code);
		Response getFileResponse(std::string path);
		Response getCreateFileResponse(Request &request);
		Response getDeleteFileResponse(Request &request);

		// CGI
		bool isCGI(std::string path);
		Response execCGI(Client &client, std::string path);
		std::string getCGI(std::string path);

		// Redirection
		bool isRedirection(std::string uri) const;
		std::string getRedirection(std::string uri) const;
		
		std::string locationResolver(std::string uri) const;
		bool isAllowedMethod(Method method) const;
		static std::string getFileListing(DIR *dir, const std::string &currentUri);

	public:
		const Config config;

		Server(const Config config);
		~Server(void);
		
		int initSocket(void);
		int getSocket(void) { return _socket; }
		void closeSocket(void);
		
		std::vector<Client> &getClients(void) { return _clients; }
		void closeClient(Client &client);

		Client acceptClient(void);
		
		std::string receiveSegment(Client &client);
		void receiveFirstSegment(Client &client);
		void receiveOtherSegment(Client &client);
		bool isEndOfSegment(Client &client);
		void onSegmentTimeout(Client &client);

		bool readCGI(Client &client);
		bool onCGIOutput(Client &client);
		void onCGITimeout(Client &client);
		void stopCGI(Client &client);
		char * const *getCGIEnv(Client &client);
		
		bool onRequest(Client &client);
		
		void onKeepAliveTimeout(Client &client);

		std::string getPort(void) const;

		// exception
		class ClientDisconnected: public std::exception {
			virtual const char *what(void) const throw() { return "Client disconnected."; }
		};
		class BadRequest: public std::exception {
			virtual const char *what(void) const throw() { return "Request is not a valid HTTP Request."; }
		};
};

#endif