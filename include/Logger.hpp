/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 22:54:17 by ehode             #+#    #+#             */
/*   Updated: 2026/02/05 20:26:09 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <sstream>
# include <fstream>

# define ENDL "\n"

# define WHITE "\e[0;37m"
# define GREEN "\e[0;32m"
# define RED "\e[0;31m"
# define DARK_RED "\e[41m"
# define YELLOW "\e[0;33m"
# define RESET "\e[0m"

enum Level {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class Logger {
	private:
		std::stringstream _current;
		Level _level;
		std::ofstream _file;

		void log(Level level, std::string const &message);
		std::string getColor(Level level);
		std::string getLevelName(Level level);
	public:
		Logger(Level level = INFO);
		~Logger(void);

		void setLevel(Level level);
		void debug(std::string const &message) {this->log(DEBUG, message);}
		void info(std::string const &message) {this->log(INFO, message);}
		void warning(std::string const &message) {this->log(WARNING, message);}
		void error(std::string const &message) {this->log(ERROR, message);}
		void critical(std::string const &message) {this->log(CRITICAL, message);}

		template <typename T>
		Logger &operator<<(T value) {
			_current << value;
			return (*this);
		}
		
		Logger &operator<<(Level level);
		Logger &operator<<(const char *s);
};

extern Logger logger;

#endif