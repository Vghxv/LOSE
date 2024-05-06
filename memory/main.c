#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <virtual address>\n", argv[0]);
        return 1;
    }
    printf("Virtual address: %s\n", argv[1]);
    unsigned long address = strtoul(argv[1], NULL, 0);
    printf("Page number: %lu\n", address >> 12);
    printf("Offset: %lu\n", address & 0xfff);
    return 0;
}