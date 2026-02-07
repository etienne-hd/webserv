/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 22:45:36 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 22:46:30 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Method.hpp"

#include <stdexcept>

Method getMethodFromString(const std::string &s) {
	if (s == "GET")
		return (GET);
	else if (s == "POST")
		return (POST);
	else if (s == "DELETE")
		return (DELETE);
	else
		throw std::runtime_error(std::string("Invalid Method: ") + s);
}