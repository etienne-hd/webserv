/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:44:28 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 22:26:00 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Request.hpp"
# include "Response.hpp"
# include <ostream>
# include <string>
# include <iostream>

class Client {
	public:
		int				socket;
		unsigned int	raw_address;
		std::string		address;
		unsigned short	raw_port;
		unsigned int	total_request;
		long			client_timeout;

		Request			request;
		Response		response;
	
	
		Client(int socket, unsigned int rawAddress, unsigned short rawPort);

		void resetIO(void) { this->request = Request(); this->response = Response(); }
		
		bool operator==(const Client &client);
};

std::ostream &operator<<(std::ostream &stream, const Client &client);

#endif