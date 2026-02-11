/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 19:26:38 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 19:57:44 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Headers.hpp"
#include <cstdlib>

bool Headers::has(std::string key) {
	return (this->find(key) != this->end());
}

unsigned long Headers::getContentLength(void) {
	std::string contentLengthStr = (*this)["content-length"];
	if (contentLengthStr.empty())
		contentLengthStr = "0";
	char *endptr;
	unsigned long contentLength = std::strtoul(contentLengthStr.c_str(), &endptr, 0);
	if (*endptr != '\0')
		contentLength = 0;
	return (contentLength);
}