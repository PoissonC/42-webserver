/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Settings.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:05:06 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/30 18:16:39 by yu               ###   ########.fr       */
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

void parseConfig(const std::vector<std::string> &tokens, std::vector<ServerConfig> &servers);
std::vector<std::string> tokenize(const std::string &input);