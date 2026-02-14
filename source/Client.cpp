/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:14:35 by ehode             #+#    #+#             */
/*   Updated: 2026/02/14 20:24:07 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "utils.hpp"

Client::Client(int socket, unsigned int rawAddress, unsigned short rawPort) {
	this->socket = socket;
	this->raw_address = rawAddress;
	this->address = decimalToAddress(rawAddress);
	this->raw_port = rawPort;
	this->port = decimalToPort(rawPort);
	this->client_timeout = time(__null);
	this->total_request = 0;
}

bool Client::operator==(const Client &client) {
	return (
		this->raw_address == client.raw_address &&
		this->socket == client.socket
	);
}

std::ostream &operator<<(std::ostream &stream, const Client &client) {
	stream << client.address << ":" << client.port;
	return (stream);
}