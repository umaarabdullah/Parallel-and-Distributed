#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define SERVER_IP "192.168.22.10"
// #define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return 1;
    }
#endif

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return 1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("connection failed");
        return 1;
    }

    printf("Connected to the server\n");

    char ackMsg[] = "ack";  // ack and nack will keep the client informed whether client gave the correct response to server or not
    bool ackFlag = true;
    int sequence_num = 0;
    char clientTerminationMsg[] = "I have no more jokes to tell.\n";

    // Send and receive messages
    while (1) {
        // Receive a response from the server
        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            break;  // Server closed the connection or an error occurred
        }
        printf("Server: %s", buffer);

        if (strcmp(buffer, clientTerminationMsg) == 0)
            break;  // shutdown client

        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer

        if (ackFlag) {  // send response to server
            if (sequence_num == 2) {  // sequence_num implies (Y/N) question so check response validity on the client side
                while (1) {
                    printf("You: ");
                    fgets(buffer, BUFFER_SIZE, stdin);  // fgets takes newline as input
                    if (buffer[0] == 'n' || buffer[0] == 'N' || buffer[0] == 'y' || buffer[0] == 'Y') {
                        if (buffer[1] == '\n')
                            break;
                        else
                            printf("Server: Invalid Response! Only (Y/N) response applicable.\n");
                    } else {
                        printf("Server: Invalid Response! Only (Y/N) response applicable.\n");
                    }
                }
            } else {
                printf("You: ");
                fgets(buffer, BUFFER_SIZE, stdin);  // fgets takes newline as input
            }

            // Send the message to the server
            send(sock, buffer, strlen(buffer), 0);
        } else {  // Receive a response from the server

            memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer
            valread = recv(sock, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) {
                break;  // Server closed the connection or an error occurred
            }
            printf("Server: %s", buffer);
            memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer

            printf("You: ");
            fgets(buffer, BUFFER_SIZE, stdin);  // fgets takes newline as input
            // Send the message to the server
            send(sock, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer
        }

        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer
        // check for acknowledgment
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            break;  // Server closed the connection or an error occurred
        }
        if (strcmp(buffer, ackMsg) == 0) {  // ack received
            ackFlag = true;
            sequence_num++;
            if (sequence_num == 3)  // each jokes can go upto highest sequence number 3 then a new joke will start
                sequence_num = 0;
        } else {  // nack received
            ackFlag = false;
            sequence_num = 0;
        }
    }

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return 0;
}
