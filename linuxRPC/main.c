#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


#define BUFFER_SIZE 1024
pthread_mutex_t print_mutex;


char *linux_tcp_reciver();
void linux_tcp_sender(const char *receiver_ip, int port, const void *data, size_t data_length);

void *start_receiver_thread(void *arg) {
    printf("Starting TCP receiver...\n");
    while (1) {
        char *received_data = linux_tcp_reciver();

        if (received_data != NULL) {
            pthread_mutex_lock(&print_mutex);
            printf("Received data: %s\n", received_data);
            pthread_mutex_unlock(&print_mutex);
            free(received_data); // Free the memory allocated for the received data
        } else {
            pthread_mutex_lock(&print_mutex);
            printf("No data received or an error occurred.\n");
            pthread_mutex_unlock(&print_mutex);
        }
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

void *start_sender_thread(void *arg) {
    const char *receiver_ip = "192.168.50.150"; // Replace with the IP address of the receiver
    int port = 3016; // Replace with the port number the receiver is listening on
    char data[BUFFER_SIZE];

    while (1) {
        pthread_mutex_lock(&print_mutex);
        printf("Enter the data to send: ");
        fgets(data, BUFFER_SIZE, stdin);
        pthread_mutex_unlock(&print_mutex);

        size_t data_length = strlen(data);
        if (data[data_length - 1] == '\n') {
            data[data_length - 1] = '\0'; // Remove the newline character
            data_length--; // Update data length
        }

        printf("Sending data to server...\n");
        linux_tcp_sender(receiver_ip, port, data, data_length + 1); // Include the null terminator
        printf("Data sent successfully.\n");

        usleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

int main() {
    pthread_t receiver_tid, sender_tid;
    pthread_mutex_init(&print_mutex, NULL);

    // 创建接收线程
    if (pthread_create(&receiver_tid, NULL, start_receiver_thread, NULL) != 0) {
        perror("pthread_create (receiver)");
        return 1;
    }

    // 创建发送线程
    if (pthread_create(&sender_tid, NULL, start_sender_thread, NULL) != 0) {
        perror("pthread_create (sender)");
        return 1;
    }

    // 等待接收线程结束
    pthread_join(receiver_tid, NULL);

    // 等待发送线程结束
    pthread_join(sender_tid, NULL);

    return 0;
}