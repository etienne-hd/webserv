/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:18 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 22:43:15 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Headers.hpp"
# include "Cookies.hpp"

# include <string>

class Response {
	private:
		int			_status_code;
		Headers		_headers;
		Cookies		_cookies;
		std::string _content;

	public:
		Response(int status_code, Headers headers, Cookies cookies, std::string content);
		std::string build(void);

		int getStatusCode(void) const { return _status_code; }
		Headers getHeaders(void) const { return _headers; }
		Cookies getCookies(void) const { return _cookies; }
		std::string getContent(void) const { return _content; }
};

#endif