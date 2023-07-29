#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <winsock2.h>
#include <process.h> // For _beginthreadex
#include "sqlite/sqlite3.h"

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

/*********Function Prototypes********************************/
// Function to handle client connections in a separate thread
unsigned __stdcall clientThread(void* param);
bool caseInsensitiveStringCompare(const char* str1, const char* str2);
void removeNewline(char* str);

int main() {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    /* SQLite db handling */
    sqlite3* db;
    int rc = sqlite3_open("database/knock_knock_jokes.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sqlite3_stmt* stmt;
    const char* query = "SELECT * FROM setup;";

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    // exception handling
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    // while (sqlite3_step(stmt) == SQLITE_ROW) {
    //     // Fetch data here and print or process as needed
    //     int id = sqlite3_column_int(stmt, 0);
    //     const char* setup_text = (const char*)sqlite3_column_text(stmt, 1);
    //     const char* response_text = (const char*)sqlite3_column_text(stmt, 2);

    //     printf("ID: %d\n", id);
    //     printf("Setup: %s\n", setup_text);
    //     printf("Response: %s\n", response_text);
    // }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    

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

void removeNewline(char* str){
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

// Function to check if two strings are equal (case-insensitive)
bool caseInsensitiveStringCompare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (tolower(*str1) != tolower(*str2)) {
            return false;
        }
        str1++;
        str2++;
    }
    // If both strings have reached the end, they are equal
    return (*str1 == '\0' && *str2 == '\0');
}

unsigned __stdcall clientThread(void* param) {
    int client_socket = (int)(uintptr_t)param;
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    bool firstMessageFlag = true;
    bool correctResponseFlag = false;

    // Receive and send messages
    while (1) {

        // send joke to client
        if(firstMessageFlag){
            char joke[] = "Knock knock!\n";
            send(client_socket, joke, strlen(joke), 0);
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

            // recieve response from client
            valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) {
                break; // Client disconnected or an error occurred
            }
            printf("Client: %s", buffer);
            
            // check spelling of response message from client
            removeNewline(buffer);
            if(caseInsensitiveStringCompare(buffer, "Who's there?")){
                correctResponseFlag = true;
                firstMessageFlag = false;
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, "ack");  // acknowledgement
                send(client_socket, buffer, strlen(buffer), 0);
            }
            else{
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, "nack");  // non-acknowledgement
                send(client_socket, buffer, strlen(buffer), 0);

                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, "You are supposed to say, \"Who's there?\". Let's try again.\n");
                send(client_socket, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                firstMessageFlag = true;
            }
        }
        else{

        }

    }

    printf("Client disconnected\n");

    closesocket(client_socket);
    return 0;
}
