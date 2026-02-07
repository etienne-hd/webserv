/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JSONUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:20:36 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 13:11:24 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JSONReader.hpp"

#include <stdexcept>
#include <string>
#include <cstring>

static std::string::iterator checker(std::string::iterator begin, std::string::iterator end) {
	std::string::iterator it = begin;
	bool isDict = false;
	if (*it == '{' || *it == '[')
	{
		isDict = *it == '{';
		it++;
	}
	else
		throw std::runtime_error("Invalid begin");
	
	for (; it != end; ) {
		if ((*it == '}' || *it == ']')) {
			if ((*it == '}' && isDict) || (*it == ']' && !isDict))
				return (it + 1);
			throw std::runtime_error("Invalid close");
		}

		// key (must start with ")
		if (isDict) {
			bool inScope = false;
			if (*it != '"')
				throw std::runtime_error("Invalid token: key must start by \"");
			std::string key;
			while (it != end) {
				if (*it == '"' && *(it - 1) != '\\')
					inScope = !inScope;
				else
					key += *it;
				it++;
				if (inScope == false)
					break;
			}

			// next token is :
			if (*it != ':')
				throw std::runtime_error("Invalid token: expecting : after key");
			it++;
		}

		// value
		std::string value;
		// Dict / Array
		if (*it == '[' || *it == '{')
		{
			std::string::iterator endValue = checker(it, end);
			value = std::string(it, endValue);
			it = endValue;
		// String
		} else if (*it == '"') {
			bool inScope = false;
			while (it != end) {
				if (*it == '"' && *(it - 1) != '\\')
					inScope = !inScope;
				else
					value += *it;
				it++;
				if (inScope == false)
					break;
			}
		// Number
		} else if (std::isdigit(*it) || *it == '-') {
			if (*it == '-')
			{
				value += *it;
				it++;
			}
			int count = 0;
			while (std::isdigit(*it) || *it == '.')
			{
				if (*it == '.')
					count++;
				if (*it == '.' && !(std::isdigit(*(it - 1)) && std::isdigit(*(it + 1))))
					throw std::runtime_error("Invalid Number: invalid dot position");
				value += *it;
				it++;
			}
			if (count > 1)
				throw std::runtime_error("Invalid number: too many dot");
		// Bool
		} else if (std::strncmp("false", std::string(it, end).c_str(), 5) == 0 ||
			std::strncmp("true", std::string(it, end).c_str(), 4) == 0) {
			value = std::strncmp("false", std::string(it, end).c_str(), 5) == 0 ? "false" : "true";
			it += value.length();
		} else if (std::strncmp("null", std::string(it, end).c_str(), 4) == 0) {
			value = "null";
			it += value.length();
		} else {
			throw std::runtime_error("Invalid value");
		}
		
		// next token is , }, ]
		if (*it == ',' || *it == '}' || *it == ']') {
			if (*it == ',')
				it++;
		} else
			throw std::runtime_error("Invalid token: expecting [',', '}', ']'] after value");
	}
	return (it);
}

bool JSONReader::isValidJSON(std::string data) {
	bool inScope = false;
	for (std::string::iterator it = data.begin(); it != data.end(); it++) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else if (inScope == false && ((*it >= 8 && *it <= 13) || *it == ' ' || *it == '\n'))
			data.erase(it--);
	}
	try {
		if (checker(data.begin(), data.end()) != data.end())
			return (false);
	} catch (std::exception &e) {
		return (false);
	}
	return (true);
}