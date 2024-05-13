#include <stdint.h>
#include <stdio.h>

#define TEST_LOW 0b00
#define TEST_HIGH 0b101<<3

int main() {
    // Example uint16_t value
    uint16_t value = TEST_LOW | TEST_HIGH;

    char test[] = "hello";
    
    // Print the string
    printf("String %s %#x\n", 0x01);

    return 0;
}
