/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 04:28:41 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 20:35:27 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <sys/socket.h>

std::string Server::getRawRequest(int clientSocket) {
	// 8192 -> Method + Uri + HTTP Version + Headers
	char *buffer = new char[_config.max_body_size + 8192 + 2];
	
	int code = recv(clientSocket, buffer, _config.max_body_size + 8192 + 1, 0);
	if (code == 0) {
		delete[] buffer;
		throw Server::ClientDisconnected();
	}
	if (code == -1) {
		delete[] buffer;
		throw std::runtime_error("recv error");
	}
	buffer[code] = 0;
	
	std::string rawRequest = buffer;
	delete[] buffer;
	
	if (rawRequest.length() > _config.max_body_size + 8192 + 1)
		throw Server::RequestEntityTooLarge();

	return (rawRequest);
}