/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:05:27 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/10 12:59:31 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>

bool	Server::_constructed = false;

static void close_fds(std::vector<int> & fds) {
	for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); it++) {
		close(*it);
	}
}

Server::Server(std::vector<Settings> & servers) {
	if (_constructed)
		throw AlreadyConstructed();
	_constructed = true;

	// create epoll instance
	_epoll_fd = epoll_create(1);
	if (_epoll_fd == -1)
		throw CreatEpollFail();

	// set socket for all servers, bind and listen.
	{
		int on = 1;
		for (std::vector<Settings>::iterator it = servers.begin(); it != servers.end(); it++) {
			int new_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
			if (new_socket_fd < 0) {
				close(_epoll_fd);
				throw SocketFail();
			}
			_servers_fd.push_back(new_socket_fd);
			if (setsockopt(new_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
				close_fds(_servers_fd);
				close(_epoll_fd);
				throw SetSockOptFail();
			}
			if (bind(new_socket_fd, (struct sockaddr *)&(it->_addr), sizeof(it->_addr)) < 0) {
				close_fds(_servers_fd);
				close(_epoll_fd);
				throw BindFail();
			}
			if (listen(new_socket_fd, BACK_LOG) < 0) {
				close_fds(_servers_fd);
				close(_epoll_fd);
				throw ListenFail();
			}
			
		}
	}
}

// Exceptions

const char* Server::AlreadyConstructed::what() const throw() {
	return "The server instance is already constructed.";
}

const char* Server::CreatEpollFail::what() const throw() {
	return "epoll_create() failed.";
}

const char* Server::SocketFail::what() const throw() {
	return "socket() failed";
}

const char* Server::SetSockOptFail::what() const throw() {
	return "setsockopt() failed.";
}

const char* Server::BindFail::what() const throw() {
	return "bind() failed.";
}

const char* Server::ListenFail::what() const throw() {
	return "listen() failed.";
}

