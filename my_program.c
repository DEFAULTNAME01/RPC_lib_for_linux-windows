#include "random_bytes.h"
#include <stdio.h>
void generate_random_bytes(unsigned char* buffer, size_t size);
int main() {
    const size_t buffer_size = 16;
    unsigned char buffer[buffer_size];
    generate_random_bytes(buffer, buffer_size);
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%02x ", buffer[i]);
    }
    printf("\n");
    return 0;
}