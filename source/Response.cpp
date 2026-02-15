/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:17:45 by ehode             #+#    #+#             */
/*   Updated: 2026/02/15 19:00:52 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#include "Cookies.hpp"
#include "Logger.hpp"
#include "status_code.hpp"
#include "utils.hpp"

#include <map>
#include <sstream>
#include <string>

Response::Response(void) {
	this->status_code = 200;
	this->content_type = "text/plain";
}

std::string Response::getStatusCodeText(int status_code) {
	std::map<int, std::string> statusCodeTexts;
	
	statusCodeTexts[RESPONSE_CONTINUE] = "Continue";
	statusCodeTexts[RESPONSE_SWITCHING_PROTOCOLS] = "Switching Protocols";
	statusCodeTexts[RESPONSE_PROCESSING] = "Processing";
	statusCodeTexts[RESPONSE_EARLY_HINTS] = "Early Hints";

	statusCodeTexts[RESPONSE_OK] = "OK";
	statusCodeTexts[RESPONSE_CREATED] = "Created";
	statusCodeTexts[RESPONSE_ACCEPT] = "Accepted";
	statusCodeTexts[RESPONSE_NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
	statusCodeTexts[RESPONSE_NO_CONTENT] = "No Content";
	statusCodeTexts[RESPONSE_RESET_CONTENT] = "Reset Content";
	statusCodeTexts[RESPONSE_PARTIAL_CONTENT] = "Partial Content";
	statusCodeTexts[RESPONSE_MULTI_STATUS] = "Multi-Status";
	statusCodeTexts[RESPONSE_ALREADY_REPORTED] = "Already Reported";
	statusCodeTexts[RESPONSE_IM_USED] = "IM Used";
	
	statusCodeTexts[RESPONSE_MULTIPLE_CHOICES] = "Multiple Choices";
	statusCodeTexts[RESPONSE_MOVED_PERMANENTLY] = "Moved Permanently";
	statusCodeTexts[RESPONSE_FOUND] = "Found";
	statusCodeTexts[RESPONSE_SEE_OTHER] = "See Other";
	statusCodeTexts[RESPONSE_NOT_MODIFIED] = "Not Modified";
	statusCodeTexts[RESPONSE_USE_PROXY] = "Use Proxy";
	statusCodeTexts[RESPONSE_UNUSED] = "unused";
	statusCodeTexts[RESPONSE_TEMPORARY_REDIRECT] = "Temporary Redirect";
	statusCodeTexts[RESPONSE_PERMANENT_REDIRECT] = "Permanent Redirect";
	
	statusCodeTexts[RESPONSE_BAD_REQUEST] = "Bad Request";
	statusCodeTexts[RESPONSE_UNAUTHORIZED] = "Unauthorized";
	statusCodeTexts[RESPONSE_PAYMENT_REQUIRED] = "Payment Required";
	statusCodeTexts[RESPONSE_FORBIDDEN] = "Forbidden";
	statusCodeTexts[RESPONSE_NOT_FOUND] = "Not Found";
	statusCodeTexts[RESPONSE_METHOD_NOT_ALLOWED] = "Method Not Allowed";
	statusCodeTexts[RESPONSE_NOT_ACCEPTABLE] = "Not Acceptable";
	statusCodeTexts[RESPONSE_PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
	statusCodeTexts[RESPONSE_REQUEST_TIMEOUT] = "Request Timeout";
	statusCodeTexts[RESPONSE_CONFLICT] = "Conflict";
	statusCodeTexts[RESPONSE_GONE] = "Gone";
	statusCodeTexts[RESPONSE_LENGTH_REQUIRED] = "Length Required";
	statusCodeTexts[RESPONSE_PRECONDITION_FAILED] = "Precondition Failed";
	statusCodeTexts[RESPONSE_CONTENT_TOO_LARGE] = "Content Too Large";
	statusCodeTexts[RESPONSE_URI_TOO_LONG] = "URI Too Long";
	statusCodeTexts[RESPONSE_UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
	statusCodeTexts[RESPONSE_RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
	statusCodeTexts[RESPONSE_EXPECTATION_FAILED] = "Expectation Failed";
	statusCodeTexts[RESPONSE_IM_A_TEAPOT] = "I'm a teapot";
	statusCodeTexts[RESPONSE_MISDIRECTED_REQUEST] = "Misdirected Request";
	statusCodeTexts[RESPONSE_UNPROCESSABLE_CONTENT] = "Unprocessable Content";
	statusCodeTexts[RESPONSE_LOCKED] = "Locked";
	statusCodeTexts[RESPONSE_FAILED_DEPENDENCY] = "Failed Dependency";
	statusCodeTexts[RESPONSE_TOO_EARLY] = "Too Early";
	statusCodeTexts[RESPONSE_UPGRADE_REQUIRED] = "Upgrade Required";
	statusCodeTexts[RESPONSE_PRECONDITION_REQUIRED] = "Precondition Required";
	statusCodeTexts[RESPONSE_TOO_MANY_REQUESTS] = "Too Many Requests";
	statusCodeTexts[RESPONSE_REQUEST_HEADER_FILEDS_TOO_LARGE] = "Request Header Fields Too Large";
	statusCodeTexts[RESPONSE_UNAVAILABLE_FOR_LEGAL_REASONS] = "Unavailable For Legal Reasons";
	
	statusCodeTexts[RESPONSE_INTERNAL_SERVER_ERROR] = "Internal Server Error";
	statusCodeTexts[RESPONSE_NOT_IMPLEMENTED] = "Not Implemented";
	statusCodeTexts[RESPONSE_BAD_GATEWAY] = "Bad Gateway";
	statusCodeTexts[RESPONSE_SERVICE_UNAVAILABLE] = "Service Unavailable";
	statusCodeTexts[RESPONSE_GATEWAY_TIMEOUT] = "Gateway Timeout";
	statusCodeTexts[RESPONSE_HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
	statusCodeTexts[RESPONSE_VARIANT_ALSO_NEGOTIATES] = "Variant Also Negotiates";
	statusCodeTexts[RESPONSE_INSUFFICIENT_STORAGE] = "Insufficient Storage";
	statusCodeTexts[RESPONSE_LOOP_DETECTED] = "Loop Detected";
	statusCodeTexts[RESPONSE_NOT_EXTENDED] = "Not Extended";
	statusCodeTexts[RESPONSE_NETWORK_AUTHENTICATION_REQUIRED] = "Network Authentication Required";

	std::string statusCodeText = statusCodeTexts[status_code];
	if (statusCodeText.empty()) {
		logger << WARNING << "Status code " << status_code << " text not implemented!" << ENDL;
		return ("Unknown");
	}
	return (statusCodeText);
}

void Response::setContentTypeByPath(std::string path) {
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

	std::string extension = getFileExtension(path);
	std::string contentType = contentTypes[extension];
	if (contentType.empty())
		logger << WARNING << "Unable to find content type of '" << extension << "' extension." << ENDL;
	else
		this->content_type = contentType;
}

std::string Response::build(void) {
	std::stringstream s;

	s << "HTTP/1.1" << " ";
	s << this->status_code << " ";
	s << getStatusCodeText(this->status_code) << "\r\n";

	s << "Server: " << "webserv" << "\r\n";
	s << "Content-Length: " << this->content.length() << "\r\n";
	s << "Content-Type: " << this->content_type << "\r\n";

	if (!this->cookies.empty()) {
		for (Cookies::iterator cookie = this->cookies.begin(); cookie != this->cookies.end(); cookie++) {
			s << "Set-Cookie: " << cookie->first << "=" << cookie->second << "\r\n";
		}
	}
	for (Headers::iterator header = this->headers.begin(); header != this->headers.end(); header++) {
		s << header->first << ": " << header->second << "\r\n";
	}
	
	s << "\r\n";
	s << this->content;
	return (s.str());
}