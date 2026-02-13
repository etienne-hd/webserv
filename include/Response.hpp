/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:18 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 22:19:49 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "CGI.hpp"
# include "Cookies.hpp"
# include "Headers.hpp"

# include <string>

class Response {
	public:
		int			status_code;
		Headers		headers;
		Cookies		cookies;
		std::string	content;
		std::string	content_type;

		CGI			cgi;

		Response(void);
		
		std::string build(void);

		void setContentTypeByPath(std::string path);
		static std::string getStatusCodeText(int status_code);

};

#endif