/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 22:59:59 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 00:07:08 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include <ctime>
#include <fstream>
#include <iostream>

Logger logger(DEBUG);

static std::string getFormatedTime(std::string const fmt) {
	time_t timestamp = time(NULL);
	struct tm datetime = *localtime(&timestamp);

	char tmp[1024];
	std::strftime(tmp, sizeof(tmp), fmt.c_str(), &datetime);
	return (std::string(tmp));
}

Logger::Logger(Level level) {
	_level = level;

	std::string fileName = "log/";
	fileName.append(getFormatedTime("%Y-%m-%d_%H-%M-%S"));
	fileName.append(".log");
	_file.open(fileName.c_str());
	if (!_file.is_open())
		this->error("Unable to open log file.");
}

Logger::~Logger(void) {
	if (_file.is_open())
		_file.close();
}

void Logger::setLevel(Level level) {
	_level = level;
}

void Logger::log(Level level, std::string const &message) {
	if (_level > level)
		return;

	std::string currentTime = getFormatedTime("%Y-%m-%d %H:%M:%S");
	std::string logName;
	std::string color;

	if (level == DEBUG) {
		logName = "DEBUG";
		color = WHITE;
	} else if (level == INFO) {
		logName = "INFO";
		color = GREEN;
	} else if (level == WARNING) {
		logName = "WARNING";
		color = YELLOW;
	} else if (level == ERROR) {
		logName = "ERROR";
		color = RED;
	} else if (level == CRITICAL) {
		logName = "CRITICAL";
		color = DARK_RED;
	} else {
		logName = "UNKNOWN";
		color = WHITE;
	}
	if (level == ERROR || level == CRITICAL)
		std::cerr << "[" << currentTime << "] " << color << "[" << logName << "] " << message << RESET << std::endl;
	else
		std::cout << "[" << currentTime << "] " << color << "[" << logName << "] " << message << RESET << std::endl;
	if (_file.is_open())
		_file << "[" << currentTime << "] " << "[" << logName << "] " << message << std::endl;
}