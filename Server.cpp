/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychen2 <ychen2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:05:27 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/09 23:02:21 by ychen2           ###   ########.fr       */
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
	_epoll_fd = epoll_create1(0);
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
			rc = bind(new_socket_fd, (struct sockaddr *)&(it->_addr), sizeof(addr));
			if (rc < 0)
			{
				perror("bind() failed");
				close(listen_sd);
				exit(-1);
			}

			/* Set the listen back log */
			rc = listen(listen_sd, 32);
			if (rc < 0)
			{
				perror("listen() failed");
				close(listen_sd);
				exit(-1);
			}
			
		}
	}
}

// Exceptions

const char* Server::AlreadyConstructed::what() const throw() {
	return "The server instance is already constructed.";
}

const char* Server::CreatEpollFail::what() const throw() {
	return "epoll_create1() failed.";
}

const char* Server::SocketFail::what() const throw() {
	return "socket() failed";
}

const char* Server::SetSockOptFail::what() const throw() {
	return "setsockopt() failed.";
}