#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BYTE_SIZE 8

void generate_random_bytes(unsigned char* buffer, size_t size) {
    srand(time(NULL));
    for (size_t i = 0; i < size; i++) {
        unsigned char byte = 0;
        for (int j = 0; j < BYTE_SIZE; j++) {
            byte |= (rand() & 1) << j;
        }
        buffer[i] = byte;
    }
}