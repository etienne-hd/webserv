/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:03 by ehode             #+#    #+#             */
/*   Updated: 2026/02/12 22:01:07 by ehode            ###   ########.fr       */
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
	private:
		Method								_method;
		std::string							_raw_method;
		std::string							_uri;
		std::string							_raw_uri;
		std::string							_http_version;
		Headers								_headers;
		std::map<std::string, std::string>	_parameters;
		std::string							_raw_parameters;
		std::string							_content;
		
		// prevent from reading more segment
		int									_pre_response_status_code;

		// Segment
		unsigned int						_segment_count;
		long								_segment_timeout;

		void initParameters(void);
		void initUri(void);

	public:
		Request(void);
		
		Method &getMethod(void) { return _method; }
		std::string &getRawMethod(void) { return _raw_method; }
		std::string &getUri(void) { return _uri; }
		std::string &getRawUri(void) { return _raw_uri; }
		std::string &getHTTPVersion(void) { return _http_version; }
		Headers &getHeaders(void) { return _headers; }
		std::map<std::string, std::string> &getParameters(void) { return _parameters; }
		std::string &getRawParameters(void) { return _raw_parameters; }
		std::string &getContent(void) { return _content; }
		
		// Error handling
		int &getPreResponseStatusCode(void) { return _pre_response_status_code; }

		// Segment
		unsigned int &getSegmentCount(void) { return _segment_count; }
		long &getSegmentTimeout(void) { return _segment_timeout; }
	};

#endif