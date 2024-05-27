#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void linux_tcp_sender(const char *reciver_ip, int port, const void *data, size_t data_length) {
    int send_fd;
    struct sockaddr_in server_addr;

    // 创建套接字
    if ((send_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, reciver_ip, &server_addr.sin_addr);

    if (connect(send_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // 发送数据
    send(send_fd, data, data_length, 0);

    // 关闭套接字
    close(send_fd);
}