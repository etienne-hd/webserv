/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 04:08:33 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Method.hpp"
# include "Cookies.hpp"
# include "Headers.hpp"

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
};

#endif