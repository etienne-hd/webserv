/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 20:26:08 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Method.hpp"

# include <string>
# include <map>

enum RequestToken {
	METHOD,
	URI,
	HTTP_VERSION,
	HEADERS,
	CONTENT
};

class Request {
	private:
		Method _method;
		std::string _raw_method;
		std::string	_uri;
		std::string _http_version;
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _cookies;
		std::map<std::string, std::string> _parameters;
		std::string _content;

	public:
		Request(std::string rawRequest);
		
		Method getMethod(void) const { return _method; }
		std::string getRawMethod(void) const { return _raw_method; }
		std::string getUri(void) const { return _uri; }
		std::map<std::string, std::string> getHeaders(void) const { return _headers; }
		std::map<std::string, std::string> getCookies(void) const { return _cookies; }
		std::map<std::string, std::string> getParameters(void) const { return _parameters; }
		std::string getContent(void) const { return _content; }
};

#endif