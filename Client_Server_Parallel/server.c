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
#define jokeDatabaseSize 30

/*********Function Prototypes********************************/
// Function to handle client connections in a separate thread
unsigned __stdcall clientThread(void* param);
bool caseInsensitiveStringCompare(const char* str1, const char* str2);
void removeNewline(char* str);
bool areAllJokesVisited(const bool visitedJokes[], int size);

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

bool areAllJokesVisited(const bool visitedJokes[], int size){
    for (int i = 1; i <= size; i++) {
        if (!visitedJokes[i]) {
            return false; // At least one joke is not visited
        }
    }
    return true; // All jokes are visited
}

unsigned __stdcall clientThread(void* param) {
    int client_socket = (int)(uintptr_t)param;
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    bool firstMessageFlag = true;
    bool visitedJoke[jokeDatabaseSize + 1];     // as db id starts from 1
    memset(visitedJoke, false, jokeDatabaseSize + 1);

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
        else{   // fetch jokes from database

            unsigned jokeID;
            char setup_text_str[300];
            char response_text_str[300];
            char punchline_text_str[300];

            // If all jokes are visited then send termination msg to client
            if(areAllJokesVisited(visitedJoke,jokeDatabaseSize)){       
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, "I have no more jokes to tell.\n");
                send(client_socket, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                break;
            }

            /******************SQLite Database handling********************************/
            sqlite3* db;
            int rc = sqlite3_open("database/knock_knock_jokes.db", &db);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return 1;
            }

            sqlite3_stmt* stmt;

            int random_joke_number = rand() % jokeDatabaseSize + 1;
            while(visitedJoke[random_joke_number])       // jokes must not repeat and client must terminate when all jokes are shown
                random_joke_number = rand() % jokeDatabaseSize + 1;
            
            char query[300]; // Adjust the buffer size as per your query length
            sprintf(query, "SELECT s.setup_text, s.response_text, p.punchline_text \
                            FROM setup s \
                            INNER JOIN punchline p ON s.id = p.setup_id \
                            WHERE s.id = %d;", random_joke_number);

            rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

            // exception handling
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return 1;
            }

            if (sqlite3_step(stmt) == SQLITE_ROW) {
                // Fetch data
                int id = random_joke_number;
                const char* setup_text = (const char*)sqlite3_column_text(stmt, 0);
                const char* response_text = (const char*)sqlite3_column_text(stmt, 1);
                const char* punchline_text = (const char*)sqlite3_column_text(stmt, 2);

                jokeID = id;
                strcpy(setup_text_str, setup_text);
                strcpy(response_text_str, response_text);
                strcpy(punchline_text_str, punchline_text);

                visitedJoke[jokeID] = true;
                
            } else {
                printf("Joke with ID %d not found in the database.\n", random_joke_number);
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);

            printf("Setup: %s\n", setup_text_str);
            printf("Response: %s\n", response_text_str);
            printf("Punchline: %s\n", punchline_text_str);

            // send joke to client
            strcat(setup_text_str, "\n");
            send(client_socket, setup_text_str, strlen(setup_text_str), 0);

            // recieve response from client
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
            valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) {
                break; // Client disconnected or an error occurred
            }
            printf("Client: %s", buffer);
            
            // check spelling of response message from client
            removeNewline(buffer);
            if(caseInsensitiveStringCompare(buffer, response_text_str)){
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, "ack");  // acknowledgement
                send(client_socket, buffer, strlen(buffer), 0);

                // append the punchline with question Y/N whether client wants more jokes or no
                char final_punchline[500]; // Adjust the buffer size as needed
                strcpy(final_punchline, punchline_text_str);
                strcat(final_punchline, "\nServer: Would you like to listen to another? (Y/N)\n");

                // send the punchline
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, final_punchline);
                send(client_socket, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

                // receive response from client
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (valread <= 0) {
                    break; // Client disconnected or an error occurred
                }
                printf("Client: %s", buffer);
                removeNewline(buffer);
                if(buffer[0] == 'n' || buffer[0] == 'N'){
                    memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                    firstMessageFlag = false;
                    // send nack
                    memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                    strcpy(buffer, "nack");  // non-acknowledgement
                    send(client_socket, buffer, strlen(buffer), 0);
                    break;
                }
                else{
                    firstMessageFlag = true;    // flag to start another knock knock joke
                    // send ack
                    memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                    strcpy(buffer, "ack");  // acknowledgement
                    send(client_socket, buffer, strlen(buffer), 0);
                }
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
            }
            else{
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                strcpy(buffer, "nack");  // non-acknowledgement
                send(client_socket, buffer, strlen(buffer), 0);

                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                sprintf(buffer, "You are supposed to say, \"%s\". Let's try again.\n", response_text_str);
                send(client_socket, buffer, strlen(buffer), 0);
                memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                visitedJoke[jokeID] = false;    // mark joke as not visited
                firstMessageFlag = true;
                break;
            }
        }

    }

    printf("Client disconnected\n");

    closesocket(client_socket);
    return 0;
}
