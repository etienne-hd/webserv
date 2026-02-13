/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 20:49:43 by ehode             #+#    #+#             */
/*   Updated: 2026/02/13 23:13:01 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "JSONReader.hpp"
#include "Method.hpp"

#include <exception>
#include <stdexcept>
#include <vector>

Config::Config(
	std::string name,
	unsigned int keepalive_timeout,
	unsigned int max_body_size,
	std::string listen,
	std::map<std::string, std::string> locations,
	std::string document_index,
	std::map<int, std::string> error_pages,
	std::vector<Method> allowed_methods,
	std::map<std::string, std::string> redirections,
	bool directory_listing_enabled,
	std::string file_on_directory,
	bool file_upload_enabled,
	std::string file_upload_directory,
	bool cgi_enabled,
	std::map<std::string, std::string> cgi_rules,
	int cgi_timeout
):
name(name),
keepalive_timeout(keepalive_timeout), 
max_body_size(max_body_size),
listen(listen), locations(locations),
document_index(document_index),
error_pages(error_pages),
allowed_methods(allowed_methods),
redirections(redirections),
directory_listing_enabled(directory_listing_enabled),
file_on_directory(file_on_directory),
file_upload_enabled(file_upload_enabled),
file_upload_directory(file_upload_directory),
cgi_enabled(cgi_enabled),
cgi_rules(cgi_rules),
cgi_timeout(cgi_timeout) {}

Config Config::getConfig(JSONReader reader) {
	const char *requiredKeys[] = {
		"name",
		"listen",
		"locations",
		NULL
	};
	std::vector<std::string> keys = reader.keys();
	
	// Check required keys
	for (unsigned int i = 0; requiredKeys[i] != __null ; i++) {
		const char *requiredKey = requiredKeys[i];
		std::vector<std::string>::iterator key = keys.begin();
		for (; key != keys.end(); key++) {
			if (*key == requiredKey)
				break;
		}
		if (key == keys.end())
			throw std::runtime_error(std::string("The key ") + requiredKey + " is required!");
	}
	
	// Init variable
	std::string							name;
	std::string							listen;
	std::map<std::string, std::string>	locations;
	
	unsigned int						keepalive_timeout = 30;
	unsigned int						max_body_size = 32768;
	std::string							document_index = "index.html";
	std::map<int, std::string>			error_pages;
	std::vector<Method>					allowed_methods;
	allowed_methods.push_back(GET);
	allowed_methods.push_back(POST);
	allowed_methods.push_back(DELETE);
	std::map<std::string, std::string>	redirections;
	bool								directory_listing_enabled = false;
	std::string							file_on_directory = "directory.html";
	bool								file_upload_enabled = false;
	std::string							file_upload_directory = ".";
	bool								cgi_enabled = false;
	std::map<std::string, std::string>	cgi_rules;
	int									cgi_timeout = 15;

	for (std::vector<std::string>::iterator key = keys.begin(); key != keys.end(); key++) {
		if (*key == "name")
			name = reader["name"].toString();
		else if (*key == "listen")
			listen = reader["listen"].toString();
		else if (*key == "locations") {
			std::vector<JSONReader> values = reader["locations"].values();
			for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
				locations[(*value)["uri"].toString()] = (*value)["path"].toString();
			}
		} else if (*key == "keepalive_timeout")
			keepalive_timeout = reader["keepalive_timeout"].toInt();
		else if (*key == "max_body_size")
			max_body_size = reader["max_body_size"].toInt();
		else if (*key == "document_index")
			document_index = reader["document_index"].toString();
		else if (*key == "error_pages") {
			std::vector<JSONReader> values = reader["error_pages"].values();
			for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
				error_pages[(*value)["error_code"].toInt()] = (*value)["path"].toString();
			}
		} else if (*key == "allowed_methods") {
			allowed_methods.clear();
			std::vector<JSONReader> values = reader["allowed_methods"].values();
			for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
				allowed_methods.push_back(getMethodFromString(value->toString()));
			}
		} else if (*key == "redirections") {
			std::vector<JSONReader> values = reader["redirections"].values();
			for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
				redirections[(*value)["uri"].toString()] = (*value)["redirect"].toString();
			}
		} else if (*key == "directory_listing_enabled") 
			directory_listing_enabled = reader["directory_listing_enabled"].toBool();
		else if (*key == "file_on_directory")
			file_on_directory = reader["file_on_directory"].toString();
		else if (*key == "file_upload_enabled")
			file_upload_enabled = reader["file_upload_enabled"].toBool();
		else if (*key == "file_upload_directory")
			file_upload_directory = reader["file_upload_directory"].toString();
		else if (*key == "cgi_enabled")
			cgi_enabled = reader["cgi_enabled"].toBool();
		else if (*key == "cgi_rules") {
			std::vector<JSONReader> values = reader["cgi_rules"].values();
			for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
				cgi_rules[(*value)["extension"].toString()] = (*value)["path"].toString();
			}
		} else if (*key == "cgi_timeout") {
			cgi_timeout = reader["cgi_timeout"].toInt();
		} else {
			throw std::runtime_error(std::string("Unknown key '") + *key + "'.");
		}
	}

	return (Config(
		name,
		keepalive_timeout,
		max_body_size,
		listen,
		locations,
		document_index,
		error_pages,
		allowed_methods,
		redirections,
		directory_listing_enabled,
		file_on_directory,
		file_upload_enabled,
		file_upload_directory,
		cgi_enabled,
		cgi_rules,
		cgi_timeout
	));
}

std::vector<Config> Config::getConfigs(std::string data) {
	std::vector<Config> configs;
	try {
		JSONReader reader(data);
		std::vector<JSONReader> values = reader.values();
		if (values.empty())
			throw std::runtime_error("provided configuration is empty.");
		if (values.size() > 10)
			throw std::runtime_error("too many configuration.");
		for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
			configs.push_back(getConfig(*value));
		}
	} catch (JSONReader::InvalidJSON &e) {
		throw std::runtime_error("provided configuration is not a valid json.");
	} catch (JSONReader::InvalidConversion &e) {
		throw std::runtime_error("unexpected value type.");
	} catch (JSONReader::JSONReaderError &e) {
		throw std::runtime_error("provided json contains invalid configuration.");
	} catch (std::exception &e) {
		throw std::runtime_error(e.what());
	}
	return (configs);
}