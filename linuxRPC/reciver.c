#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 3015
#define BUFFER_SIZE 1024

char* linux_tcp_reciver() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size;
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    ssize_t bytes_received;

    // 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置套接字选项
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 绑定套接字
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 监听套接字
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    client_addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_size);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // 接收数据
while (1) {
    client_addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_size);

    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    memset(buffer, 0, BUFFER_SIZE);
    bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received < 0) {
        perror("recv");
        free(buffer);
        buffer = NULL;
    } else {
        printf("Received data: %s\n", buffer);
    }

    // 关闭客户端套接字
    close(client_fd);
    printf("Client disconnected\n");
}

    // 关闭服务器套接字
    close(server_fd);

    return buffer;
}   