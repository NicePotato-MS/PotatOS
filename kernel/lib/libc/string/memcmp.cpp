#include <stddef.h>
#include <string.h>

/* Compare the first n bytes of memory areas pointed to by ptr1 and ptr2 */
int memcmp(const void *ptr1, const void *ptr2, size_t n) {
    // Cast the void pointers to unsigned char pointers for byte-wise comparison
    const unsigned char *byte_ptr1 = (const unsigned char *)ptr1;
    const unsigned char *byte_ptr2 = (const unsigned char *)ptr2;

    // Compare each byte up to n
    for (size_t i = 0; i < n; ++i) {
        if (byte_ptr1[i] < byte_ptr2[i]) {
            return -1;
        } else if (byte_ptr1[i] > byte_ptr2[i]) {
            return 1;
        }
        // If the bytes are equal, continue comparing the next byte
    }

    return 0;  // The memory blocks are equal up to the first n bytes
}
