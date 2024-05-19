/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yu <yu@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:05:27 by ychen2            #+#    #+#             */
/*   Updated: 2024/05/16 15:47:37 by yu               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <utility>
#include <cerrno>
#include <cstdio>
#include "Server.hpp"
#include "State.hpp"

bool	Server::_constructed = false;

static void close_fds(std::vector<int> & fds) {
	for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); it++) {
		close(*it);
	}
}

static bool is_socket(std::vector<int> & socks, int fd) {
	return std::find(socks.begin(), socks.end(), fd) == socks.end() ? false : true;
}

static std::vector< std::pair<int, t_state> >::iterator get_state(std::vector< std::pair<int, t_state> > & states, int fd) {
	for (std::vector <std::pair<int, t_state> >::iterator it = states.begin(); it != states.end(); it++) {
		if (it->first == fd)
			return it;
	}
	return states.end();
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
		for (std::vector<Settings>::iterator it = servers.begin(); it != servers.end(); it++) {
			int new_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
			if (new_socket_fd < 0) {
				close(_epoll_fd);
				throw SocketFail();
			}
			_socks_fd.push_back(new_socket_fd);
			{
				int on = 1;
				if (setsockopt(new_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
					close_fds(_socks_fd);
					close(_epoll_fd);
					throw SetSockOptFail();
				}
			}
			if (bind(new_socket_fd, (struct sockaddr *)&(it->_addr), sizeof(it->_addr)) < 0) {
				close_fds(_socks_fd);
				close(_epoll_fd);
				throw BindFail();
			}
			if (listen(new_socket_fd, BACK_LOG) < 0) {
				close_fds(_socks_fd);
				close(_epoll_fd);
				throw ListenFail();
			}
			// Setting up sockets for epoll
			{
				struct epoll_event ev;
				ev.events = EPOLLIN;
				ev.data.fd = new_socket_fd;
				if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, new_socket_fd, &ev) < 0) {
					close_fds(_socks_fd);
					close(_epoll_fd);
					throw EpollCtlFail();
				}
			}
		}
	}
}

void	Server::run() {
	struct epoll_event	events[MAX_EVENTS], ev;
	int					nfds;
	std::vector< std::pair<int, t_state> >	states;

	while (1) {
		nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS, -1);
		if (nfds == -1)
			throw EpollWaitFail();
		// Process all returned events
		for (int i = 0; i < nfds; i++) {
			// Check if the event is for a server socket
			if (is_socket(_socks_fd, events[i].data.fd)) {
				int new_sd;
				// Accept new connections
				while ((new_sd = accept(events[i].data.fd, NULL, NULL)) != -1) {
					ev.events = EPOLLIN;
					ev.data.fd = new_sd;
					if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, new_sd, &ev) == -1)
						throw EpollCtlFail();
					t_state	new_conn;
					states.push_back(std::make_pair(new_sd, new_conn));
				}
				if (errno != EWOULDBLOCK)
					throw AcceptFail();
			}
			// If the event is not in _socks_fd, handle recv/send
			else {
				char												buffer[BUFFER_SIZE];
				std::vector< std::pair<int, t_state> >::iterator	cur_state;
				bool												close_conn = false;

				cur_state = get_state(states, events[i].data.fd);
				if (events[i].events & EPOLLOUT) {
					// Handle write event
					int	wc;
					if (cur_state->second.buffer.size()) {
						wc = send(events[i].data.fd, cur_state->second.buffer.c_str(), cur_state->second.buffer.size(), 0);
					}
					if (wc < 0) {
						perror("send() failed");
						close_conn = true;
					}
					else  {
						// erase the buffer anyway, and keep the connection open
						cur_state->second.buffer.erase(0, wc);
						// If the buffer is empty, we wait for the next request
						if (cur_state->second.buffer.empty()) {
							ev.events = EPOLLIN;
							ev.data.fd = cur_state->first;
							if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, cur_state->first, &ev) == -1)
								throw EpollCtlFail();
						}
					}
				}
				else if (events[i].events & EPOLLIN) {
					int	rc;
					if (cur_state == states.end())
						throw std::runtime_error("State not found");
					rc = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
					if (rc < 0) {
						perror("recv() failed");	
						close_conn = true;
					}
					else {
						// If reading ends.
						if (rc == 0) {
							// Handle the request (not implemented yet)


							// Replace the buffer with the data to send
							cur_state->second.buffer = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 13\n\nHello World!\n";
						}
						else {
							cur_state->second.buffer.append(buffer, rc);
							std::cout  <<"Request: " << cur_state->second.buffer << std::endl;
							// this means the reading ends.
							if (rc < BUFFER_SIZE) {
								// These just tests
								cur_state->second.buffer = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 13\n\nHello World!\n";
								ev.events |= EPOLLOUT;
								ev.data.fd =cur_state->first;
								if (epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, cur_state->first, &ev) == -1)
									throw EpollCtlFail();
							}

							std::cout <<"Respond: "<< cur_state->second.buffer << std::endl;
						}
					}
					
				}
				
				// Close connection if any error occurs (http/1.1 keeps the connection)
				if (close_conn) {
					close(events[i].data.fd);
					epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
					// This should destroy the cur_state element (free resourses, Chat GPT says so)
					states.erase(cur_state);
				}
			}
		}
	}
}

Server::~Server() {
	close_fds(_socks_fd);
	close(_epoll_fd);
}

// Exceptions

const char* Server::AlreadyConstructed::what() const throw() {
	return "The server instance is already constructed.";
}

const char* Server::CreatEpollFail::what() const throw() {
	return "epoll_create() failed.";
}

const char* Server::EpollCtlFail::what() const throw() {
	return "epoll_ctl() failed";
}

const char* Server::EpollWaitFail::what() const throw() {
	return "epoll_wait() failed";
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

const char* Server::AcceptFail::what() const throw() {
	return "accept() failed.";
}
