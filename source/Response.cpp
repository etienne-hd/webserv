/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:17:45 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 21:39:24 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Cookies.hpp"
#include "Logger.hpp"
#include "status_code.hpp"

#include <map>
#include <sstream>
#include <string>

Response::Response(void) {
	_status_code = 200;
	_content_type = "text/plain";
}

std::string Response::getStatusCodeText(int status_code) {
	std::map<int, std::string> statusCodeTexts;
	
	statusCodeTexts[RESPONSE_OK] = "OK";
	statusCodeTexts[RESPONSE_NO_CONTENT] = "No Content";
	statusCodeTexts[RESPONSE_CREATED] = "Created";
	
	statusCodeTexts[RESPONSE_MOVED_PERMANENTLY] = "Moved Permanently";
	
	statusCodeTexts[RESPONSE_BAD_REQUEST] = "Bad Request";
	statusCodeTexts[RESPONSE_UNAUTHORIZED] = "Unauthorized";
	statusCodeTexts[RESPONSE_FORBIDDEN] = "Forbidden";
	statusCodeTexts[RESPONSE_NOT_FOUND] = "Not Found";
	statusCodeTexts[RESPONSE_REQUEST_TIMEOUT] = "Request Timeout";
	statusCodeTexts[RESPONSE_CONTENT_TOO_LARGE] = "Content Too Large";
	statusCodeTexts[RESPONSE_TOO_MANY_REQUESTS] = "Too Many Requests";
	
	statusCodeTexts[RESPONSE_INTERNAL_SERVER_ERROR] = "Internal Server Error";
	statusCodeTexts[RESPONSE_NOT_IMPLEMENTED] = "Not Implemented";
	statusCodeTexts[RESPONSE_BAD_GATEWAY] = "Bad Gateway";
	statusCodeTexts[RESPONSE_SERVICE_UNAVAILABLE] = "Service Unavailable";
	statusCodeTexts[RESPONSE_HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";

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