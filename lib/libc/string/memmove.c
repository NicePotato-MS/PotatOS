#include <stddef.h>
#include <string.h>

/* Copy n bytes from src to dest, handling overlapping regions */
void *memmove(void *dest, const void *src, size_t n) {
    // Cast the void pointers to char pointers for byte-wise movement
    char *dest_ptr = (char *)dest;
    const char *src_ptr = (const char *)src;

    // Check if the source and destination regions overlap
    if (src_ptr < dest_ptr) {
        // If they do, copy from the end to the beginning to avoid overwriting data
        for (size_t i = n; i > 0; --i) {
            dest_ptr[i - 1] = src_ptr[i - 1];
        }
    } else if (src_ptr > dest_ptr) {
        // If they do not overlap, copy from the beginning to the end
        for (size_t i = 0; i < n; ++i) {
            dest_ptr[i] = src_ptr[i];
        }
    }
    // If src and dest are the same or n is 0, no action is needed

    return dest;
}

