/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 19:56:16 by ehode             #+#    #+#             */
/*   Updated: 2026/02/15 20:15:05 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <dirent.h>
# include <netinet/in.h>
# include <sstream>
# include <string>

template <typename T>
void delete_vector(T &vector) {
	for (typename T::iterator it = vector.begin(); it != vector.end(); it++) {
		delete *it;
	}
	vector.clear();
} 

sockaddr_in getSocketAddress(std::string interface);
std::string decimalToAddress(unsigned int rawAddress);
std::string decimalToPort(unsigned short rawPort);
bool isSamePath(std::string path1, std::string path2);
DIR *getDirectory(const std::string &path);
std::string readFile(std::string fileName);
void strToLower(std::string &s);
void strToUpper(std::string &s);
std::string getFileExtension(const std::string &path);
void removeDuplicateSlash(std::string &s);
bool expectedToken(std::string &s, std::string::iterator &it, std::string expectedToken);
std::string getHostname(void);
bool isValidUri(std::string uri);

template <typename T>
std::string toString(T v) {
	std::stringstream ss;
	ss << v;
	return (ss.str());
}

#endif