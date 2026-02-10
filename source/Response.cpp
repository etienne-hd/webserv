/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:17:45 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 13:06:35 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Cookies.hpp"
#include "Logger.hpp"

#include <map>
#include <sstream>
#include <string>

static std::string getStatusCodeText(int status_code) {
	std::map<int, std::string> statusCodeTexts;
	
	statusCodeTexts[200] = "OK";
	statusCodeTexts[204] = "No Content";
	
	statusCodeTexts[301] = "Moved Permanently";
	
	statusCodeTexts[400] = "Bad Request";
	statusCodeTexts[401] = "Unauthorized";
	statusCodeTexts[403] = "Forbidden";
	statusCodeTexts[404] = "Not Found";
	statusCodeTexts[408] = "Request Timeout";
	statusCodeTexts[413] = "Content Too Large";
	statusCodeTexts[429] = "Too Many Requests";
	
	statusCodeTexts[500] = "Internal Server Error";
	statusCodeTexts[501] = "Not Implemented";
	statusCodeTexts[502] = "Bad Gateway";
	statusCodeTexts[505] = "HTTP Version Not Supported";

	std::string statusCodeText = statusCodeTexts[status_code];
	if (statusCodeText.empty()) {
		logger << WARNING << "Status code " << status_code << " text not implemented!" << ENDL;
		return ("Unknown");
	}
	return (statusCodeText);
}

Response::Response(void) {
	_status_code = 200;
}

std::string Response::build(void) {
	std::stringstream s;

	s << "HTTP/1.1" << " ";
	s << _status_code << " ";
	s << getStatusCodeText(_status_code) << "\r\n";
	
	s << "Server: " << "WebServ" << "\r\n";
	s << "Content-Length: " << _content.length() << "\r\n";
	if (!_cookies.empty()) {
		for (Cookies::iterator cookie = _cookies.begin(); cookie != _cookies.end(); cookie++) {
			s << "Set-Cookie: " << cookie->first << "=" << cookie->second << "\r\n";
		}
	}
	for (Headers::iterator header = _headers.begin(); header != _headers.end(); header++) {
		s << header->first << ": " << header->second << "\r\n";
	}
	
	s << "\r\n";
	s << _content;
	return (s.str());
}