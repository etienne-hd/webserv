/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:44:28 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 20:50:53 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "Request.hpp"
# include <ostream>
# include <string>
# include <iostream>

class Client {
	private:
		int				_socket;
		unsigned int	_raw_address;
		std::string		_address;
		unsigned short	_raw_port;
		unsigned int	_total_request;
		long			_client_timeout;
		Request			_request;
	
	public:
		Client(int socket, unsigned int rawAddress, unsigned short rawPort);

		int getSocket(void) const { return _socket; }
		unsigned int getRawAddress(void) const { return _raw_address; }
		std::string getAddress(void) const { return _address; }
		unsigned int getRawPort(void) const { return _raw_port; }
		long &getClientTimeout(void) { return _client_timeout; }
		unsigned int &getTotalRequest(void) { return _total_request; }
		Request &getRequest(void) { return _request; };
		void resetRequest(void) { _request = Request(); }
		
		bool operator==(const Client &client);
};

std::ostream &operator<<(std::ostream &stream, const Client &client);

#endif