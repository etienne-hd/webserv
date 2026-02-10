/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 19:58:41 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 08:50:39 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "Logger.hpp"

#include <cctype>
#include <cstdlib>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

static unsigned short getPort(std::string interface) {
	unsigned long portPos = interface.find(":");
	if (portPos == std::string::npos) {
		throw std::runtime_error("Port not found!");
	}
	std::string portStr = std::string(interface.begin() + portPos + 1, interface.end());
	for (std::string::iterator it = portStr.begin(); it != portStr.end(); it++) {
		if (!std::isdigit(*it))
			throw std::runtime_error("Invalid port");
	}
	if (portStr.empty())
		throw std::runtime_error("No port provided");
	int port = std::atoi(portStr.c_str());
	if (port > 65536)
		throw std::runtime_error("The port cannot be greater than 65536!");
	logger << DEBUG << "Port: " << port << ENDL;
	return (htons(port));
}

static unsigned int getAddress(std::string interface) {
	unsigned long AddressEndPos = interface.find(":");
	if (AddressEndPos == std::string::npos) {
		throw std::runtime_error("Address not found!");
	}
	std::string addressStr = std::string(interface.begin(), interface.begin() + AddressEndPos);
	int dotCount = 0;
	for (std::string::iterator it = addressStr.begin(); it != addressStr.end(); it++) {
		if (!(std::isdigit(*it) || *it == '.'))
			throw std::runtime_error("Invalid address");
		if (*it == '.')
			dotCount++;
	}
	if (dotCount > 3)
		throw std::runtime_error("Invalid address"); 

	std::stringstream ss(addressStr);
	unsigned int address = 0;
	std::string valueStr;
	unsigned int i = 0;
	while (std::getline(ss, valueStr, '.')) {
		if (valueStr.empty() || valueStr.size() > 3 || i > 3)
			throw std::runtime_error("Invalid address");
		int value = std::atoi(valueStr.c_str());
		if (value > 255)
			throw std::runtime_error("Invalid address");
		address += value << 8 * i;
		i++;
	}
	logger << DEBUG << "Address: " << addressStr << "; Binary: " << address << ENDL;
	return (address);
}

sockaddr_in getSocketAddress(std::string interface) {
	logger << DEBUG << "Interface: " << interface << ENDL;
	sockaddr_in addr;
	
	addr.sin_family = AF_INET;
	addr.sin_port = getPort(interface);
	addr.sin_addr.s_addr = getAddress(interface);
	return (addr);
}

// i = 0
// 1 2 3 4
// << (3 - i) * 8
// 4 0 0 0
// >> 3 * 8
// 0 0 0 4
std::string decimalToAddress(unsigned int rawAddress) {
	std::stringstream address;

	for (unsigned int i = 0; i < 4; i++) {
		address << ((rawAddress << ((3 - i) * 8)) >> 24);
		if (i != 3)
			address << ".";
	}
	return (address.str());
}