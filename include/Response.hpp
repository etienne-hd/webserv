/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:18 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 14:51:17 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Cookies.hpp"
# include "Headers.hpp"

# include <string>

class Response {
	private:
		int			_status_code;
		Headers		_headers;
		Cookies		_cookies;
		std::string	_content;
		std::string	_content_type;

	public:
		Response(void);
		std::string build(void);

		int &getStatusCode(void) { return _status_code; }
		Headers &getHeaders(void) { return _headers; }
		Cookies &getCookies(void) { return _cookies; }
		std::string &getContent(void) { return _content; }
		std::string &getContentType(void) { return _content_type; }
		void setContentTypeByPath(std::string path);
};

#endif