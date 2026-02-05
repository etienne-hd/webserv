/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 22:59:59 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 20:26:37 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <ctime>
# include <iostream>

# include "Logger.hpp"

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

std::string Logger::getColor(Level level) {
	std::string color;

	if (level == DEBUG) {
		color = WHITE;
	} else if (level == INFO) {
		color = GREEN;
	} else if (level == WARNING) {
		color = YELLOW;
	} else if (level == ERROR) {
		color = RED;
	} else if (level == CRITICAL) {
		color = DARK_RED;
	} else {
		color = WHITE;
	}

	return (color);
}

std::string Logger::getLevelName(Level level) {
	std::string levelName;

	if (level == DEBUG) {
		levelName = "DEBUG";
	} else if (level == INFO) {
		levelName = "INFO";
	} else if (level == WARNING) {
		levelName = "WARNING";
	} else if (level == ERROR) {
		levelName = "ERROR";
	} else if (level == CRITICAL) {
		levelName = "CRITICAL";
	} else {
		levelName = "UNKNOWN";
	}
	return (levelName);
}

void Logger::log(Level level, std::string const &message) {
	if (_level > level)
		return;

	std::string currentTime = getFormatedTime("%Y-%m-%d %H:%M:%S");
	std::string levelName = getLevelName(level);
	std::string color = getColor(level);

	if (level == ERROR || level == CRITICAL)
		std::cerr << "[" << currentTime << "] " << color << "[" << levelName << "] " << RESET << message << std::endl;
	else
		std::cout << "[" << currentTime << "] " << color << "[" << levelName << "] " << RESET << message << std::endl;
	if (_file.is_open())
		_file << "[" << currentTime << "] " << "[" << levelName << "] " << message << std::endl;
}

Logger &Logger::operator<<(Level level) {
	_level = level;
	return (*this);
}

Logger &Logger::operator<<(const char *s) {
	if (std::string(s) == "\n")
	{
		this->log(_level, _current.str());
		_current.str("");
	}
	else
		_current << s;
	return (*this);
}