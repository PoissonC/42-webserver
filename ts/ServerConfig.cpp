/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:21:05 by yu                #+#    #+#             */
/*   Updated: 2024/05/19 13:54:27 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : _listen("0.0.0.0:80"), _root("./") {
}

std::vector<std::string>	ServerConfig::getServerNames() const {
	return _server_name;
}

std::string	ServerConfig::getListen() const {
	return _listen;
}

std::string	ServerConfig::getRoot() const {
	return _root;
}

std::map<int, std::string>	ServerConfig::getErrorPages() const {
	return _error_pages;
}

std::map<std::string, LocationConfig>	ServerConfig::getLocations() const {
	return _locations;
}

void	ServerConfig::setListen(const std::vector<std::string> &tokens, size_t &pos) {
	_listen = tokens[pos++];
	if (tokens[pos++] != ";")
		throw std::runtime_error("Expected ';' after listen");
}

void	ServerConfig::setRoot(const std::vector<std::string> &tokens, size_t &pos) {
	_root = tokens[pos++];
	DIR* dir = opendir(_root.c_str());
	if (dir == NULL)
		throw std::runtime_error("Root directory error");
	closedir(dir);
	if (tokens[pos++] != ";")
		throw std::runtime_error("Expected ';' after root");
}

void	ServerConfig::addServerName(const std::vector<std::string> &tokens, size_t &pos) {
	while (tokens[pos] != ";" && pos < tokens.size()) {
		_server_name.push_back(tokens[pos++]);
	}
	if (pos == tokens.size())
		throw std::runtime_error("Expected ';' after server name");
	pos++; // skip ';'
}

void	ServerConfig::addErrorPage(const std::vector<std::string> &tokens, size_t &pos) {
	int code = 0;
	for (size_t i = 0; i < tokens[pos].size(); i++) {
		if (tokens[pos][i] < '0' || tokens[pos][i] > '9')
			throw std::runtime_error("Invalid status code: " + tokens[pos]);
		code = code * 10 + tokens[pos][i] - '0';
	}
	for (std::map<int, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); it++) {
		if (it->first == code)
			throw std::runtime_error("Duplicate status code");
	}
	if (code < 100 || code > 599)
		throw std::runtime_error("Invalid status code");
	_error_pages[code] = tokens[++pos];
	if (tokens[++pos] != ";")
		throw std::runtime_error("Expected ';' after error page");
	pos++; // skip ';'
}

void	ServerConfig::addLocation(const std::string & path, const LocationConfig &location) {
	if (_locations.find(path) != _locations.end())
		throw std::runtime_error("Location already exists");
	_locations[path] = location;
}


bool ServerConfig::hasRootLocation() const {
	if (_locations.empty())
		return false;
	for (std::map<std::string, LocationConfig>::const_iterator it = _locations.begin(); it != _locations.end(); it++) {
		if (it->first == "/")
			return true;
	}
	return false;
}

ServerConfig	parseServer(const std::vector<std::string> &tokens, size_t &pos) {
	ServerConfig server;

	while (pos < tokens.size() && tokens[pos] != "}") {
		if (tokens[pos] == "server_name") {
			if (pos + 1 == tokens.size())
				throw std::runtime_error("Expected server name");
			server.addServerName(tokens, ++pos);
		}
		else if (tokens[pos] == "listen") {
			if (pos + 1 == tokens.size())
				throw std::runtime_error("Expected listen address");
			server.setListen(tokens, ++pos);
		}
		else if (tokens[pos] == "root") {
			if (pos + 1 == tokens.size())
				throw std::runtime_error("Expected root directory");
			server.setRoot(tokens, ++pos);
		}
		else if (tokens[pos] == "error_page") {
			if (pos + 1 == tokens.size())
				throw std::runtime_error("Expected error page");
			server.addErrorPage(tokens, ++pos);
		}
		else if (tokens[pos] == "location") {
			if (pos + 1 == tokens.size())
				throw std::runtime_error("Expected location path");
			std::string path = tokens[++pos];
			if (path[0] != '/')
				throw std::runtime_error("Location path must start with '/'");
			if (tokens[++pos] != "{")
				throw std::runtime_error("Expected '{' after 'location'");
			LocationConfig location = parseLocation(tokens, ++pos);
			server.addLocation(path, location);
		}
		else {
			throw std::runtime_error("Unexpected token in server block '" + tokens[pos] + "'");
		}
	}
	if (pos == tokens.size())
		throw std::runtime_error("Expected '}' at the end of server block");
	pos++; // skip '}'
	if (server.hasRootLocation() == false)
		throw std::runtime_error("Server block must have a root location '/'");

	return server;
}