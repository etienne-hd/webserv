/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 19:22:10 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "Headers.hpp"
# include "Method.hpp"

#include <exception>
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
		std::string _raw_uri;
		std::string _http_version;
		Headers _headers;
		std::map<std::string, std::string> _parameters;
		std::string _raw_parameters;
		std::string _content;

		void initParameters(void);
		void initUri(void);

	public:
		Request(void);
		Request(std::string rawRequest);
		
		Method getMethod(void) const { return _method; }
		std::string getRawMethod(void) const { return _raw_method; }
		std::string getUri(void) const { return _uri; }
		std::string getRawUri(void) const { return _raw_uri; }
		std::string getHTTPVersion(void) const { return _http_version; }
		Headers getHeaders(void) const { return _headers; }
		std::map<std::string, std::string> getParameters(void) const { return _parameters; }
		std::string getRawParameters(void) const { return _raw_parameters; }
		std::string getContent(void) const { return _content; }

		// exception
		class BadRequest: public std::exception {
			virtual const char *what(void) const throw() { return "Request is not a valid HTTP Request."; }
		};
	};

#endif