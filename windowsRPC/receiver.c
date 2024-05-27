#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 3016
#define BUFFER_SIZE 1024

char* win_tcp_reciver() {
    WSADATA wsaData;
    int reciver_fd, send_fd;
    struct sockaddr_in reciver_addr, sender_addr;
    int sender_addr_size;
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
     char *data = NULL;
    int data_length = 0;
    int bytes_received;

    // 初始化 Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // 创建套接字
    if ((reciver_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项
    int opt = 1;
    if (setsockopt(reciver_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt)) < 0) {
        fprintf(stderr, "setsockopt failed: %d\n", WSAGetLastError());
        closesocket(reciver_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // 绑定套接字
    reciver_addr.sin_family = AF_INET;
    reciver_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    reciver_addr.sin_port = htons(PORT);

    if (bind(reciver_fd, (struct sockaddr *)&reciver_addr, sizeof(reciver_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
        closesocket(reciver_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // 监听套接字
    if (listen(reciver_fd, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "listen failed: %d\n", WSAGetLastError());
        closesocket(reciver_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("reciver is listening on port %d...\n", PORT);

    // 接受客户端连接
    sender_addr_size = sizeof(sender_addr);
    send_fd = accept(reciver_fd, (struct sockaddr *)&sender_addr, &sender_addr_size);

    if (send_fd == INVALID_SOCKET) {
        fprintf(stderr, "accept failed: %d\n", WSAGetLastError());
        closesocket(reciver_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("sender connected\n");

   while (1) {
        // 接收数据
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(send_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received == SOCKET_ERROR) {
            fprintf(stderr, "recv failed: %d\n", WSAGetLastError());
            free(buffer);
            buffer = NULL;
            break;
        } else if(bytes_received == 0) {
            printf("sender disconnected\n");
            break;
        } else {
            //printf("Received data: %s\n", buffer);
            data = realloc(data, data_length + bytes_received + 1);
            memcpy(data + data_length, buffer, bytes_received);
            data_length += bytes_received;
            data[data_length] = '\0';
        }
    }
    // 关闭客户端和套接字
closesocket(send_fd);
closesocket(reciver_fd);
WSACleanup();
    // 返回数据
     free(buffer);
     return data;
}