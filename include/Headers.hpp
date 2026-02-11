/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:32:30 by ehode             #+#    #+#             */
/*   Updated: 2026/02/11 19:34:16 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
# define HEADERS_HPP

# include <map>
# include <string>

class Headers: public std::map<std::string, std::string> {
	public:
		bool has(std::string key);
		unsigned long getContentLength(void);
};

#endif