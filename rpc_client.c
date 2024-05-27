#include <stdio.h>
#include <stdlib.h>
#include <grpc/grpc.h>
#include <grpcpp/client_context.h>
#include "random_bytes.h"
#include <conio.h>

int main(int argc, char** argv) {
    // Create a gRPC channel to connect to the server.
    grpc::ChannelArguments channel_args;
    channel_args.SetInt(GRPC_ARG_MAX_RECEIVE_MESSAGE_LENGTH, -1);
    channel_args.SetInt(GRPC_ARG_MAX_SEND_MESSAGE_LENGTH, -1);
    auto channel = grpc::CreateCustomChannel("192.168.50.19:50051", grpc::InsecureChannelCredentials(), channel_args);
    auto stub = RandomBytes::NewStub(channel);

    // Wait for the user to press 'k' to start the RPC.
    printf("Press 'k' to start the RPC...\n");
    while (true) {
        char c = _getch();
        if (c == 'k') break;
    }

    // Create a new RPC context and request message.
    grpc::ClientContext context;
    GetRandomBytesRequest request;
    request.set_size(1024);  // Generate 1024 random bytes.

    // Call the 'get_random_bytes' RPC service to get random bytes from the server.
    GetRandomBytesResponse response;
    grpc::Status status = stub->get_random_bytes(&context, request, &response);

    // Print the received random bytes to the console.
    if (status.ok()) {
        printf("Received random bytes:\n");
        fwrite(response.bytes().c_str(), sizeof(unsigned char), response.bytes().length(), stdout);
        fflush(stdout);
    } else {
        printf("Error: %s\n", status.error_message().c_str());
    }

    // Wait for the user to press 's' to stop the RPC.
    printf("\n\nPress 's' to stop the RPC...\n");
    while (true) {
        char c = _getch();
        if (c == 's') break;
    }

    return 0;
}