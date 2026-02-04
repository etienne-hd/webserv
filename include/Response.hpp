/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:18 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 00:27:23 by ehode            ###   ########.fr       */
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
};

#endif