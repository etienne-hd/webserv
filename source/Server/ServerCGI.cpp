/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCGI.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:43:44 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 16:55:13 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <map>
#include <string>

bool Server::isCGI(std::string path) {
	std::string extension = getFileExtension(path);
	for (std::map<std::string, std::string>::const_iterator it = _config.cgi_rules.begin(); it != _config.cgi_rules.end(); it++) {
		if (it->first == extension)
			return (true);
	}
	return (false);
}

