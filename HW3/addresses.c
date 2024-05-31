#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096 // 4KB page size
#define PAGE_MASK 0xFFFFF000 // Mask for page number
#define OFFSET_MASK 0x00000FFF // Mask for offset within page

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <virtual_address>\n", argv[0]);
        return 1;
    }

    unsigned int address = atoi(argv[1]); // Convert input string to integer

    // Extract page number and offset
    unsigned int page_number = address >> 12; // Shift right by 12 bits to get page number
    unsigned int offset = address & OFFSET_MASK; // Mask to get offset within page

    printf("The address %u contains:\n", address);
    printf("page number=%u\n", page_number);
    printf("offset=%u\n", offset);

    return 0;
}
