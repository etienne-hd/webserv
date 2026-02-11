/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:17:45 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 20:08:42 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Cookies.hpp"
#include "Logger.hpp"

#include <map>
#include <sstream>
#include <string>

Response::Response(void) {
	_status_code = 200;
	_content_type = "text/plain";
}

std::string Response::getStatusCodeText(int status_code) {
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

void Response::setContentTypeByPath(std::string path) {
	std::string extension;

	for (std::string::iterator it = path.begin(); it != path.end(); it++) {
		if (*it == '.')
			extension.clear();
		extension += *it;
	}

	std::map<std::string, std::string> contentTypes;
	
	contentTypes[".htm"] = "text/html";
	contentTypes[".html"] = "text/html";
	contentTypes[".json"] = "application/json";
	contentTypes[".js"] = "text/javascript";
	contentTypes[".css"] = "text/css";
	contentTypes[".jpeg"] = "image/jpeg";
	contentTypes[".jpg"] = "image/jpeg";
	contentTypes[".png"] = "image/png";
	contentTypes[".pdf"] = "application/pdf";
	contentTypes[".mp3"] = "audio/mpeg";
	contentTypes[".mp4"] = "video/mp4";
	contentTypes[".txt"] = "text/plain; charset=UTF-8";

	std::string contentType = contentTypes[extension];
	if (contentType.empty())
		logger << WARNING << "Unable to find content type of " << extension << " extension." << ENDL;
	else
		_content_type = contentType;
}

std::string Response::build(void) {
	std::stringstream s;

	s << "HTTP/1.1" << " ";
	s << _status_code << " ";
	s << getStatusCodeText(_status_code) << "\r\n";

	s << "Server: " << "webserv" << "\r\n";
	s << "Content-Length: " << _content.length() << "\r\n";
	s << "Content-Type: " << _content_type << "\r\n";

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