/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:12:16 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 01:05:41 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Server.hpp"

# include <vector>

class ServerManager {
	private:
		std::vector<Server *> _servers;
	
	public:
		ServerManager(std::vector<Server *> servers);

		// run function
		// open server socket
		// use select to manage fd when it come to read / write it
		void run(void);
};

#endif