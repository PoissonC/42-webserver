/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 21:08:04 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/30 18:16:36 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Settings.hpp"
#include <cstdlib>

std::vector<std::string> tokenize(const std::string &input) {
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
		std::vector<std::string> tmp;
		size_t pos = 0;
		while (pos < token.size()) {
			if (token[pos] == ';') {
				tmp.push_back(";");
				pos++;
			}
			else if (token[pos] == '{') {
				tmp.push_back("{");
				pos++;
			}
			else if (token[pos] == '}') {
				tmp.push_back("}");
				pos++;
			}
			else {
				size_t end = token.find_first_of(";{}", pos);
				tmp.push_back(token.substr(pos, end - pos));
				pos = end;
			}
		}
		for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end(); it++) {
			if (*it != "")
				tokens.push_back(*it);
		}
	}
	return tokens;
}

void parseConfig(const std::vector<std::string> &tokens, std::vector<ServerConfig> &servers) {
	size_t pos = 0;
	while (pos < tokens.size()) {
		if (tokens[pos++] == "server") {
			if (tokens[pos++] != "{") {
				throw std::runtime_error("Expected '{' after 'server'");
			}
			servers.push_back(parseServer(tokens, pos));
		}
		else {
			throw std::runtime_error("Expected 'server' at the outermost level");
		}
	}
}
