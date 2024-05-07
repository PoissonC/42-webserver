/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychen2 <ychen2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:02 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/07 16:11:43 by ychen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <exception>
#include "Settings.hpp"
#define BUFFER_SIZE 80

class Server {

	public:
		//Constructer, it creates the non-blocking epoll instance, listen to the ip/port from settings
		Server(std::vector<Settings> & servers);

		// member methods
		

		// exceptions
		class AlreadyConstructed: public std::exception {
			public:
				// The Socket instance is already constructed.
				virtual const char* what() const throw();
		};

	private:
		static bool				constructed;
		std::vector<Settings>	_servers;
		char*					buffer[BUFFER_SIZE];
	
};

