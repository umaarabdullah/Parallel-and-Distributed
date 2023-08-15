#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>

// #define SERVER_IP "127.0.0.1"
#define SERVER_IP "192.168.68.111"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        return 1;
    }

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
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
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        perror("connection failed");
        return 1;
    }

    printf("Connected to the server\n");

    printf("*********************************Test 2***************************************\n\n");
    
    char ackMsg[] = "ack";
    bool ackFlag = true;
    int sequence_num = 0;
    char clientTerminationMsg[] = "I have no more jokes to tell.\n";
    int trial_num = 1;
    printf("trial: %d\n", trial_num);

    // Send and receive messages
    while (1) {

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

        // Receive a response from the server
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            break; // Server closed the connection or an error occurred
        }
        printf("1st Server Msg: %s", buffer);

        if(strcmp(buffer, clientTerminationMsg) == 0)
            break;  // shutdown client

        // copy the setup text
        char setup[100];
        memset(setup, 0, 100);
        if(sequence_num == 1){
            buffer[strlen(buffer) - 2] = '\0';
            for(int i=0; buffer[i] != '\0'; i++){
                setup[i] = buffer[i];
            }
        }
        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

        if(ackFlag){    // send response to server
            if(sequence_num == 2){      // sequence_num implies (Y/N) question so check response validity on the client side
                while(1){
                    printf("You: ");
                    strcpy(buffer, "y\n");
                    printf("%s",buffer);

                    if(buffer[0] == 'n' || buffer[0] == 'N' || 
                        buffer[0] == 'y' || buffer[0] == 'Y'){
                            if(buffer[1] == '\n')
                                break;
                            else
                                printf("Server: Invalid Response! Only (Y/N) response applicable.\n");
                    }
                    else{
                        printf("Server: Invalid Response! Only (Y/N) response applicable.\n");
                    }
                }
            }
            else{
                if(trial_num%2 == 0){
                    printf("You: ");
                    memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                    if(sequence_num == 0){
                        strcpy(buffer, "asdf\n");   // WRONG Response
                        printf("%s",buffer);
                    }
                    if(sequence_num == 1){
                        sprintf(buffer, "%s who?\n",setup);
                        printf("%s",buffer);
                    }
                    trial_num++;
                }
                else{
                    printf("You: ");
                    memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
                    if(sequence_num == 0){
                        strcpy(buffer, "Who's there?\n");
                        printf("%s",buffer);
                    }
                    if(sequence_num == 1){
                        sprintf(buffer, "%s who?\n",setup);
                        printf("%s",buffer);
                    }
                }
            }

            // Send the message to the server
            send(sock, buffer, strlen(buffer), 0);
        }
        else{    // Receive a response from the server

            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
            
            valread = recv(sock, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) {
                break; // Server closed the connection or an error occurred
            }
            printf("2nd Server Msg: %s", buffer);
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer

            printf("You: ");
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
            if(sequence_num == 0){
                strcpy(buffer, "Who's there?\n");
                printf("%s",buffer);
            }
            if(sequence_num == 1){
                sprintf(buffer, "%s who?\n",setup);
                printf("%s",buffer);
            }
            // Send the message to the server
            send(sock, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
        }

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
        // check for acknowledgment
        valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread <= 0) {
            break; // Server closed the connection or an error occurred
        }
        if(strcmp(buffer, ackMsg) == 0){    // ack received
            ackFlag = true;
            sequence_num++;
            if(sequence_num == 3){   // each jokes can go upto highest sequence number 3 then a new trial will start
                sequence_num = 0;
                trial_num++;
                printf("trial: %d\n", trial_num);
            }
        }
        else{                               // nack received
            ackFlag = false;
            sequence_num = 0;
            trial_num++;
        }
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
