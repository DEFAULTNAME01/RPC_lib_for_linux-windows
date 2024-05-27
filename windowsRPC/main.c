#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define BUFFER_SIZE 1024

char *win_tcp_reciver();
void win_tcp_sender(const char *receiver_ip, int port, const void *data, size_t data_length);

DWORD WINAPI start_receiver_thread(LPVOID arg) {
    printf("Starting TCP receiver...\n");

    while (1) { // Add this line for an infinite loop
        char *received_data = win_tcp_reciver();

        if (received_data != NULL) {
            printf("Received data: %s\n", received_data);
            free(received_data); // Free the memory allocated for the received data
        } else {
            printf("No data received or an error occurred.\n");
        }
    }

    return 0;
}

DWORD WINAPI start_sender_thread(LPVOID arg) {
    const char *receiver_ip = "192.168.50.19"; // Replace with the IP address of the receiver
    int port = 3015; // Replace with the port number the receiver is listening on
    char data[BUFFER_SIZE];

    while (1) {
        printf("Enter the data to send: ");
        fgets(data, BUFFER_SIZE, stdin);

        size_t data_length = strlen(data);
        if (data[data_length - 1] == '\n') {
            data[data_length - 1] = '\0'; // Remove the newline character
            data_length--; // Update data length
        }

        printf("Sending data to server...\n");
        win_tcp_sender(receiver_ip, port, data, data_length + 1); // Include the null terminator
        printf("Data sent successfully.\n");
    }

    return 0;
}

int main() {
    HANDLE receiver_thread, sender_thread;

    // 创建接收线程
    receiver_thread = CreateThread(NULL, 0, start_receiver_thread, NULL, 0, NULL);
    if (receiver_thread == NULL) {
        fprintf(stderr, "Error creating receiver thread: %lu\n", GetLastError());
        return 1;
    }

    // 创建发送线程
    sender_thread = CreateThread(NULL, 0, start_sender_thread, NULL, 0, NULL);
    if (sender_thread == NULL) {
        fprintf(stderr, "Error creating sender thread: %lu\n", GetLastError());
        return 1;
    }

    // 等待接收线程结束
    WaitForSingleObject(receiver_thread, INFINITE);
    CloseHandle(receiver_thread);

    // 等待发送线程结束
    WaitForSingleObject(sender_thread, INFINITE);
    CloseHandle(sender_thread);

    return 0;
}