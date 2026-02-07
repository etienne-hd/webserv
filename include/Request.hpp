/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 22:58:24 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Method.hpp"
# include "Cookies.hpp"
# include "Headers.hpp"

#include <exception>
# include <string>
# include <map>

class Request {
	private:
		Method _method;
		std::string	_uri;
		Headers _headers;
		Cookies _cookies;
		std::map<std::string, std::string> _parameters;
		std::string _content;

	public:
		Request(std::string raw);
		
		Method getMethod(void) const { return _method; }
		std::string getUri(void) const { return _uri; }
		Headers getHeaders(void) const { return _headers; }
		Cookies getCookies(void) const { return _cookies; }
		std::map<std::string, std::string> getParameters(void) const { return _parameters; }
		std::string getContent(void) const { return _content; }

		class ClientDisconnected: public std::exception {
			virtual const char *what(void) const throw() { return "Client disconnected."; }
		};
};

#endif