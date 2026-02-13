/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 22:03:41 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "Headers.hpp"
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
	public:
		Method								method;
		std::string							raw_method;
		std::string							uri;
		std::string							raw_uri;
		std::string							http_version;
		Headers								headers;
		std::map<std::string, std::string>	parameters;
		std::string							raw_parameters;
		std::string							content;
		
		// prevent from reading more segment
		int									pre_response_status_code;

		// Segment
		unsigned int						segment_count;
		long								segment_timeout;

		Request(void);
	};

#endif