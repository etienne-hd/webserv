/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 19:56:16 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 10:47:05 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <netinet/in.h>
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
bool isSamePath(std::string path1, std::string path2);

#endif