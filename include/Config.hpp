/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:58:20 by ehode             #+#    #+#             */
/*   Updated: 2026/02/07 04:08:12 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Method.hpp"

# include <string>
# include <map>
# include <vector>

class Config {
	private:
		std::string							_name;
		unsigned int						_keepalive_timeout;
		unsigned int						_max_request_size;
		std::string							_listen;
		std::map<std::string, std::string>	_locations;
		std::string							_document_index;
		std::map<int, std::string>			_error_pages;
		std::vector<Method>					_allowed_methods;
		std::map<std::string, std::string>	_redirections;
		bool								_directory_listing_enabled;
		std::string							_file_upload_directory;
		bool								_cgi_enabled;
		std::map<std::string, std::string>	_cgi_rules;

	public:
		// getter
		std::string getName(void) const { return _name; }
		unsigned int getKeepaliveTimeout(void) const { return _keepalive_timeout; }
		unsigned int getMaxRequestSize(void) const { return _max_request_size; }
		std::string	getListenInterface(void) const { return _listen; }
		std::map<std::string, std::string> getLocations(void) const { return _locations; }
		std::string getDocumentIndex(void) const { return _document_index; }
		std::map<int, std::string> getErrorPages(void) const { return _error_pages; }
		std::vector<Method> getAllowedMethods(void) const { return _allowed_methods; } 
		std::map<std::string, std::string> getRedirections(void) const { return _redirections; }
		bool isDirectoryListingEnabled(void) const { return _directory_listing_enabled; }
		std::string getFileUploadDirectory(void) const { return _file_upload_directory; }
		bool isCgiEnabled(void) const { return _cgi_enabled; }
		std::map<std::string, std::string> getCgiRules(void) const { return _cgi_rules; }
};

#endif