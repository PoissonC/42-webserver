#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address, client_address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    unsigned char *ip;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on 0.0.0.0:8080\n");

    while (1) {
        socklen_t client_addr_len = sizeof(client_address);
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_addr_len)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Directly access the binary IP address and port
        ip = (unsigned char *)&client_address.sin_addr.s_addr;
        int client_port = ntohs(client_address.sin_port);

        printf("Accepted connection from %d.%d.%d.%d:%d\n", ip[0], ip[1], ip[2], ip[3], client_port);

        // Handle the client connection (read/write data, etc.)
        read(new_socket, buffer, 1024);
        printf("Message from client: %s\n", buffer);

        // Close the client socket
        close(new_socket);
    }

    // Close the server socket
    close(server_fd);
    return 0;
}
