#include <stddef.h>
#include <string.h>

/* Set the first n bytes of the memory area pointed to by dest to the specified value */
void *memset(void *dest, int value, size_t n) {
    // Cast the void pointer to a char pointer for byte-wise assignment
    unsigned char *dest_ptr = (unsigned char *)dest;
    
    // Fill the memory area with the specified value byte by byte
    for (size_t i = 0; i < n; ++i) {
        dest_ptr[i] = (unsigned char)value;
    }

    return dest;
}
