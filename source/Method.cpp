/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:45:36 by ehode             #+#    #+#             */
/*   Updated: 2026/02/14 20:52:27 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Method.hpp"

#include <map>
#include <stdexcept>

Method getMethodFromString(const std::string &rawMethod) {
	std::map<std::string, Method> methods;

	methods["GET"] = GET;
	methods["HEAD"] = HEAD;
	methods["POST"] = POST;
	methods["PUT"] = PUT;
	methods["DELETE"] = DELETE;
	methods["CONNECT"] = CONNECT;
	methods["OPTIONS"] = OPTIONS;
	methods["TRACE"] = TRACE,
	methods["PATCH"] = PATCH;

	if (methods.find(rawMethod) != methods.end()) {
		return (methods[rawMethod]);
	}
	throw std::runtime_error(std::string("Invalid Method: ") + rawMethod);
}