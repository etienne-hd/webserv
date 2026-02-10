/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:29:17 by ehode             #+#    #+#             */
/*   Updated: 2026/02/09 19:47:07 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "ServerManager.hpp"

#include <exception>
#include <fstream>
#include <stdexcept>
#include <vector>

const std::string readFile(std::string fileName) {
	std::fstream file(fileName.c_str());
	if (!file.is_open())
		throw std::runtime_error("Unable to open file.");

	std::string content;
	std::getline(file, content, '\0');
	return (content);
}

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		logger << ERROR << "Wrong usage: " << argv[0] << " <*.json>" << ENDL;
		return (1);
	}

	ServerManager manager;
	try {
		std::string rawConfig = readFile(argv[1]);
		std::vector<Config> configs = Config::getConfigs(rawConfig);
		for (std::vector<Config>::iterator config = configs.begin(); config != configs.end(); config++) {
			manager.addServer(Server(*config));
		}
	} catch (std::exception &e) {
		logger << ERROR << "Config Error: " << e.what() << ENDL;
		return (1);
	}

	try {
		
		manager.run();
	} catch (std::exception &e) {
		logger << ERROR << "Error: " << e.what() << ENDL;
		return (1);
	}
	return (0);
}
