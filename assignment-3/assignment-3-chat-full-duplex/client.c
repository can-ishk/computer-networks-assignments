#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "arpa/inet.h"
#include "errno.h"
#include "netdb.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h"

int main() {
    int socketDescriptor;

    struct sockaddr_in serverAddress;
    char sendBuffer[1000], recvBuffer[1000];

    pid_t cpid;

    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(5500);

    /*Creating a socket, assigning IP address and port number for that socket*/
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0) {
        printf("Error in creating socket! %s\n", strerror(errno));
        return 1;
    }
    /*Connect establishes connection with the server using server IP address*/
    int c = connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (c < 0) {
        printf("Error when connecting! %s\n", strerror(errno));
        return 1;
    }
    /*Fork is used to create a new process*/
    cpid = fork();
    if (cpid == 0) {
        while (1) {
            bzero(&sendBuffer, sizeof(sendBuffer));
            printf("\nType message to send: ");
            /*This function is used to read from server*/
            fgets(sendBuffer, 10000, stdin);
            /*Send the message to server*/
            if (send(socketDescriptor, sendBuffer, strlen(sendBuffer) + 1, 0) >= 0)
                printf("\nMessage sent!\n");
            else
                printf("\nError sending message: %s\n", strerror(errno));
        }
    } else {
        while (1) {
            bzero(&recvBuffer, sizeof(recvBuffer));
            /*Receive the message from server*/
            recv(socketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
            printf("\nMessage from Server: %s\n", recvBuffer);
        }
    }
    return 0;
}