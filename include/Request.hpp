/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 00:27:15 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Cookies.hpp"
# include "Headers.hpp"

# include <string>

enum Method {
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH
};

class Request {
	private:
		Method		_method;
		std::string	_path;
		Headers		_headers;
		Cookies		_cookies;
		std::string	_content;
};

#endif