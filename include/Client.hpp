/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:44:28 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 08:51:14 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
#include <ostream>
# include <string>
# include <iostream>

class Client {
	private:
		int				_socket;
		unsigned int	_raw_address;
		std::string		_address;
		unsigned short	_raw_port;
		unsigned int	_last_receive_request;
		unsigned int	_total_request;
	
	public:
		Client(int socket, unsigned int rawAddress, unsigned short rawPort);

		int getSocket(void) const { return _socket; }
		unsigned int getRawAddress(void) const { return _raw_address; }
		std::string getAddress(void) const { return _address; }
		unsigned int getRawPort(void) const { return _raw_port; }
		unsigned int &getLastReceiveRequest(void) { return _last_receive_request; }
		unsigned int &getTotalRequest(void) { return _total_request; }
		
		bool operator==(const Client &client);
};

std::ostream &operator<<(std::ostream &stream, const Client &client);

#endif