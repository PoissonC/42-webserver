#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define SERVER_PORT  12345

#define MAX_EVENTS 10

int main (int argc, char *argv[])
{
    int    len, rc, on = 1;
    int    listen_sd = -1, new_sd = -1;
    int    close_conn;
    char   buffer[80];
    struct sockaddr_in6   addr;
    int    epoll_fd, nfds, i;
    struct epoll_event ev, events[MAX_EVENTS];

    /* Create an AF_INET6 stream socket to receive incoming connections on */
    listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sd < 0)
    {
        perror("socket() failed");
        exit(-1);
    }

    /* Allow socket descriptor to be reusable */
    rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0)
    {
        perror("setsockopt() failed");
        close(listen_sd);
        exit(-1);
    }

    /* Set socket to be nonblocking */
    rc = ioctl(listen_sd, FIONBIO, (char *)&on);
    if (rc < 0)
    {
        perror("ioctl() failed");
        close(listen_sd);
        exit(-1);
    }

    /* Bind the socket */
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(SERVER_PORT);
    rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
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

    /* Create an epoll instance */
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1 failed");
        close(listen_sd);
        exit(-1);
    }

    /* Add the listening socket to the interest list */
    ev.events = EPOLLIN;
    ev.data.fd = listen_sd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sd, &ev) == -1)
    {
        perror("epoll_ctl: listen_sd");
        close(listen_sd);
        exit(-1);
    }

    /* The event loop */
    while (true)
    {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }

        for (i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == listen_sd)
            {
                /* Accept all incoming connections */
                while ((new_sd = accept(listen_sd, NULL, NULL)) != -1)
                {
                    ioctl(new_sd, FIONBIO, (char *)&on); // Set non-blocking mode
                    ev.events = EPOLLIN;
                    ev.data.fd = new_sd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_sd, &ev) == -1)
                    {
                        perror("epoll_ctl: add");
                        close(new_sd);
                    }
                    printf("New incoming connection - %d\n", new_sd);
                }
                if (errno != EWOULDBLOCK)
                {
                    perror("accept() failed");
                    break;
                }
            }
            else
            {
                /* Handle data from clients */
                new_sd = events[i].data.fd;
                rc = recv(new_sd, buffer, sizeof(buffer), 0);
                if (rc < 0)
                {
                    if (errno != EWOULDBLOCK)
                    {
                        perror("recv() failed");
                        close_conn = true;
                    }
                    continue;
                }
                if (rc == 0)
                {
                    printf("Connection closed\n");
                    close_conn = true;
                }
                else
                {
                    printf("%d bytes received\n", rc);
                    send(new_sd, buffer, rc, 0);
                }

                if (close_conn)
                {
                    close(new_sd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, new_sd, NULL);
                    close_conn = false;
                }
            }
        }
    }

    /* Clean up */
    close(listen_sd);
    close(epoll_fd);
    return 0;
}
