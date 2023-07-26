#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h> // For _beginthreadex
#include <sqlite3.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

// Function to handle client connections in a separate thread
unsigned __stdcall clientThread(void* param);

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to all available network interfaces
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        perror("bind failed");
        return 1;
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) == SOCKET_ERROR) {
        perror("listen failed");
        return 1;
    }

    printf("Server listening on all interfaces on port %d\n", PORT);

    // Accept incoming connections and spawn threads for each client
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) 
                == INVALID_SOCKET) {
            perror("accept failed");
            return 1;
        }

        printf("Connection established with client\n");

        // Spawn a new thread to handle the client connection
        unsigned threadId;
        _beginthreadex(NULL, 0, clientThread, (void*)(uintptr_t)new_socket, 0, &threadId);
    }

    closesocket(server_fd);
    WSACleanup();

    return 0;
}

unsigned __stdcall clientThread(void* param) {
    int client_socket = (int)(uintptr_t)param;
    char buffer[BUFFER_SIZE] = {0};
    int valread;

    // Receive and send messages
    while (1) {
        valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            break; // Client disconnected or an error occurred
        }

        printf("Client: %s", buffer);

        // Send a response back to the client
        char response[] = "Server received your message\n";
        send(client_socket, response, strlen(response), 0);

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
    }

    printf("Client disconnected\n");

    closesocket(client_socket);
    return 0;
}
