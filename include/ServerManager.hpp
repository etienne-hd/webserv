/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 04:12:16 by ehode             #+#    #+#             */
/*   Updated: 2026/02/10 17:44:30 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Server.hpp"

#include <sys/select.h>
# include <vector>

class ServerManager {
	private:
		std::vector<Server> _servers;
		fd_set _master_socket;
	
		void closeServers(void);
	public:
		ServerManager(void);

		void addServer(Server server);
		void run(void);
};

#endif