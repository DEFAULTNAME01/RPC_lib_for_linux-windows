#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void win_tcp_sender(const char *receiver_ip, int port, const void *data, size_t data_length) {
    SOCKET sockfd;
    struct sockaddr_in server_addr;
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(receiver_ip);
    server_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "connect failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Send the data
    if (send(sockfd, data, data_length, 0) == SOCKET_ERROR) {
        fprintf(stderr, "send failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Close the socket
    closesocket(sockfd);
    WSACleanup();
}


