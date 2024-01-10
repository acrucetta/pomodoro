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

// Proxy Server Workflow
// 1. Create a socket
// 2. Bind the socket to an address
// 3. Listen for connections
// 4. Accept a connection
// 5. Read from the connection (Start connection with client)
// 6. Write to the connection (Start connection with host)
// 7. Close the connection

#include "server.h"
#include <ctype.h>

const char *blocked_websites[] = {"www.wikipedia.org"};
const int NUM_BLOCKED_WEBSITES = 1;

int is_website_blocked(const char *host)
{
    int i;
    for (i = 0; i < NUM_BLOCKED_WEBSITES; ++i)
    {
        if (strcmp(host, blocked_websites[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*
Open connection to client - opens connection at <hostname, port>
*/
int client_connect(char *hostname, int port)
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }

    // Fill in the server's IP address and port
    if ((hp = gethostbyname(hostname)) == NULL)
    {
        return -2;
    }
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)hp->h_addr_list[0],
          (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
    serveraddr.sin_port = htons(port);

    // Establish a connection with the server
    if (connect(clientfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        return -1;
    }
    return clientfd;
}

/*
Handles the buffer with the HTTP request. It will:
- Read the client request
- Extract the target URL
- Open a connection with the target server
- Forward the client's request to the target server
- Forward the target server's response to the client
*/
void client_request_handler(char* buffer)
{
    return;
}

// Extract website from URL
// Example: http://www.google.com:8080/index.html
// --> www.google.com
char* extract_website(char* url)
{
    char* website = malloc(strlen(url) + 1);
    char* start = strstr(url, "//");
    if (start == NULL)
    {
        start = url;
    }
    else
    {
        start += 2;
    }
    // We're at the start of the prefix e.g., www. 
    char* end = strstr(start, ":");
    if (end == NULL)
    {
        end = strstr(start, "/");
    }
    if (end == NULL)
    {
        end = url + strlen(url);
    }
    strncpy(website, start, end - start);
    website[end - start] = '\0';
    return website;
}

/* MAIN LOOP */
int main()
{
    // Create a socket
    printf("Configuring local address...\n");
    struct sockaddr_in hints;
    memset(&hints, 0, sizeof(hints));   // Zero out the struct
    hints.sin_family = AF_INET;         // IPv4
    hints.sin_port = htons(8080);       // Port 8080
    hints.sin_addr.s_addr = INADDR_ANY; // Bind to any address

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(AF_INET, SOCK_STREAM, 0);

    if (!ISVALIDSOCKET(socket_listen))
    {
        printf("Error: could not create socket\n");
        return 1;
    }

    // Bind the socket to an address
    printf("Binding socket to local address...\n");
    bind(socket_listen, (struct sockaddr *)&hints, sizeof(hints));

    // Listen for connections
    listen(socket_listen, 10);

    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    printf("Waiting for connections...\n");

    // For v1.0, we will handle a request and print it to the console
    while (1)
    {
        fd_set reads;
        reads = master;
        if (select(max_socket + 1, &reads, 0, 0, 0) < 0)
        { // select() will wait for a connection
            printf("Error: select() failed\n");
            exit(1);
        }

        SOCKET i;
        for (i = 1; i <= max_socket; ++i)
        {
            if (FD_ISSET(i, &reads)) // Check if socket is ready
            {
                if (i == socket_listen) // New connection
                {
                    struct sockaddr_in client_address; // Client address
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_len);
                    if (!ISVALIDSOCKET(socket_client))
                    {
                        printf("Error: accept() failed\n");
                        exit(1);
                    }
                    FD_SET(socket_client, &master); // Add socket to master set
                    if (socket_client > max_socket)
                    { // Keep track of max socket, so we know when to stop searching for connections
                        max_socket = socket_client;
                    }

                    char address_buffer[100];
                    getnameinfo((struct sockaddr *)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
                    printf("New proxy connection for %s\n", address_buffer);
                }
                else
                { // Handle existing connection
                    char read[1024];
                    int bytes_received = recv(i, read, 1024, 0);
                    if (bytes_received < 1)
                    {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }
                    read[bytes_received] = '\0';
                    printf("%s", read);
                    char method[10], url[1024], protocol[10];
                    sscanf(read, "%s %s %s", method, url, protocol);
                    printf("Host --> %s", url);
                    // Just get the domain so we can see if its blocked
                    // We need to remove the http:// and the port number
                    char* website = extract_website(url);
                    if (is_website_blocked(website))
                    {
                        printf("Website is blocked\n");
                    }
                    else
                    {
                        printf("Website is not blocked\n");
                    }
                }
            }
        }
    }
    close(socket_listen);
    return 0;
}
