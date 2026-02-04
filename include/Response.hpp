/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:18 by ehode             #+#    #+#             */
/*   Updated: 2026/02/04 18:39:55 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"

#include <string>

class Response {
	private:
		int			_status_code;
		std::string _content;
		Headers		_headers;
};