// This file is a proxy server that will be used in my browser.
// It will be used to block websites that are on a list of websites.

// Proxy Server Architecture
//
// The proxy server will be a single process that will handle multiple
// connections. It will be a single-threaded server. It will be able to
// handle multiple connections by using the select() function. The select()
// function will allow the server to wait for multiple connections at the
// same time. When a connection is ready, the select() function will return
// and the server will be able to handle the connection.

// v1.0 Handle and parse HTTP requests

// Steps for a server to handle a connection:
// 1. Create a socket
// 2. Bind the socket to an address
// 3. Listen for connections
// 4. Accept a connection
// 5. Read from the connection
// 6. Write to the connection
// 7. Close the connection

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Create a socket
    printf("Configuring local address...\n");
    struct sockaddr_in server;
    server.sin_family = AF_INET; // IPv4
    server.sin_port = htons(8080); // Port 8080
    server.sin_addr.s_addr = INADDR_ANY; // Bind to any address

    printf("Creating socket...\n");
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)); // Reuse address
    if (socket_fd < 0) {
        printf("Error: could not create socket\n");
        exit(1);
    }

    // Bind the socket to an address
    printf("Binding socket to local address...\n");
    bind(socket_fd, (struct sockaddr *)&server, sizeof(server));

    // Listen for connections
    listen(socket_fd, 10);

    // For v1.0, we will handle a request and print it to the console
    while(1) {
        int client_socket_fd = accept(socket_fd, NULL, NULL);
        if (client_socket_fd < 0) {
            printf("Error: could not establish new connection\n");
            exit(1);
        }

        // Read from the connection
        char buffer[1024];
        int bytes_read = read(client_socket_fd, buffer, 1024);
        if (bytes_read < 0) {
            printf("Error: could not read from socket\n");
            exit(1);
        }

        // Print the request to the console
        printf("%s\n", buffer);

        // Write to the connection a dummy response
        char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
        int bytes_written = write(client_socket_fd, response, strlen(response));
        if (bytes_written < 0) {
            printf("Error: could not write to socket\n");
            exit(1);
        }
        close(client_socket_fd);
    }
    close(socket_fd);
    return 0;
}