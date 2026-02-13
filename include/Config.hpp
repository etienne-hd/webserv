/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:58:20 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 23:13:23 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Method.hpp"
# include "JSONReader.hpp"

# include <string>
# include <map>
# include <vector>

class Config {
	private:
		Config(
			std::string name,
			unsigned int keepalive_timeout,
			unsigned int max_body_size,
			std::string listen,
			std::map<std::string,
			std::string> locations,
			std::string document_index,
			std::map<int, std::string> error_pages,
			std::vector<Method> allowed_methods,
			std::map<std::string,
			std::string> redirections,
			bool directory_listing_enabled,
			std::string file_on_directory,
			bool file_upload_enabled,
			std::string file_upload_directory,
			bool cgi_enabled,
			std::map<std::string, std::string> cgi_rules,
			int cgi_timeout
		);

		static Config getConfig(JSONReader config);
	public:
		const std::string							name;
		const unsigned int							keepalive_timeout;
		const unsigned int							max_body_size;
		const std::string							listen;
		const std::map<std::string, std::string>	locations;
		const std::string							document_index;
		const std::map<int, std::string>			error_pages;
		const std::vector<Method>					allowed_methods;
		const std::map<std::string, std::string>	redirections;
		const bool									directory_listing_enabled;
		const std::string							file_on_directory;
		const bool									file_upload_enabled;
		const std::string							file_upload_directory;
		const bool									cgi_enabled;
		const std::map<std::string, std::string>	cgi_rules;
		const int									cgi_timeout;

		static std::vector<Config> getConfigs(std::string data);
};

#endif