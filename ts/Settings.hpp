/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:05:06 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/19 11:32:30 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <exception>

#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

class Settings {
	public:
		Settings(u_int16_t port);
		friend class Server;
	private:
		// int					_socket_fd; it depends
		struct sockaddr_in	_addr;
};

void parseConfig(const std::vector<std::string> &tokens, std::vector<ServerConfig> &servers);
std::vector<std::string> tokenize(const std::string &input);