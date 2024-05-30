/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 18:03:41 by yu                #+#    #+#             */
/*   Updated: 2024/05/30 18:07:29 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

static std::string readFile(const std::string &filename) {
    std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void	parse(std::vector<Settings> & settings, const std::string & filename) {
	std::vector<std::string> tokens = tokenize(readFile(filename));
	std::cout << "Start parsing config" << std::endl;
	std::vector<ServerConfig> servers;
	parseConfig(tokens, servers);
	std::cout << "Finish parsing config" << std::endl;
}