/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:17:45 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 20:31:11 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

#include <map>
#include <sstream>
#include <string>

Response::Response(void) {}

std::string Response::build(void) {
	std::stringstream s;

	s << "HTTP/1.1" << " ";
	s << _status_code << " ";
	s << "OK" << "\r\n";
	
	s << "Server: " << "WebServ" << "\r\n";
	s << "Content-Length: " << _content.length() << "\r\n";
	for (Headers::iterator header = _headers.begin(); header != _headers.end(); header++) {
		s << header->first << ": " << header->second << "\r\n";
	}
	
	s << "\r\n";
	s << _content;
	return (s.str());
}