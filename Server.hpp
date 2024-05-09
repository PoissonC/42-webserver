/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychen2 <ychen2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 22:00:02 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/09 22:06:21 by ychen2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <exception>
#include <sys/epoll.h>
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
				// The server instance is already constructed.
				virtual const char* what() const throw();
		};
		class CreatEpollFail: public std::exception {
			public:
				// Epoll_create1() failed
				virtual const char* what() const throw();
		};
		class SocketFail: public std::exception {
			public:
				// socket() failed
				virtual const char* what() const throw();
		};
		class SetSockOptFail: public std::exception {
			public:
				// setsockopt() failed
				virtual const char* what() const throw();
		};
	private:
		static bool				_constructed;
		int						_epoll_fd;
		std::vector<int>		_servers_fd;
	
};

