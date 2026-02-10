/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:14:35 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 09:02:30 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "utils.hpp"


Client::Client(int socket, unsigned int rawAddress, unsigned short rawPort) {
	_socket = socket;
	_raw_address = rawAddress;
	_address = decimalToAddress(rawAddress);
	_raw_port = rawPort;
	_last_receive_request = 0;
	_total_request = 0;
}

bool Client::operator==(const Client &client) {
	return (
		_raw_address == client._raw_address &&
		_socket == client._socket
	);
}

std::ostream &operator<<(std::ostream &stream, const Client &client) {
	stream << client.getAddress();
	return (stream);
}