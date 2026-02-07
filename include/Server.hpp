/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:30:31 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 22:56:49 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "Response.hpp"
# include "Request.hpp"

class Server {
	private:
		const Config &_config;
		int	_socket;
	
	public:
		Server(const Config &config);

		int initSocket(void);
		int getSocket(void) { return _socket; }
		int closeSocket(void);

		int onNewClient(void);
		Response onRequest(Request &request);
};

#endif