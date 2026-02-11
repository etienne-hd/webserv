/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:44:28 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 19:30:53 by ehode            ###   ########.fr       */
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
		std::string		_current_segment;
		unsigned int	_total_segment;
		long			_segment_timeout;
		long			_client_timeout;
	
	public:
		Client(int socket, unsigned int rawAddress, unsigned short rawPort);

		int getSocket(void) const { return _socket; }
		unsigned int getRawAddress(void) const { return _raw_address; }
		std::string getAddress(void) const { return _address; }
		unsigned int getRawPort(void) const { return _raw_port; }
		long &getClientTimeout(void) { return _client_timeout; }
		unsigned int &getTotalRequest(void) { return _total_request; }
		std::string &getRawRequest(void) { return _current_segment; }
		unsigned int &getTotalSegment(void) { return _total_segment; };
		long &getSegmentTimeout(void) { return _segment_timeout; }
		void resetSegment(void);
		Request getRequest(void);
		
		bool operator==(const Client &client);
};

std::ostream &operator<<(std::ostream &stream, const Client &client);

#endif