#include <stddef.h>
#include <string.h>

/* Copy n bytes from src to dest */
void* memcpy(void* destination, const void* source, size_t num) {
    // Cast the void pointers to char pointers for byte-wise copying
    char* dest_ptr = (char*)destination;
    const char* src_ptr = (const char*)source;

    // Copy 'num' bytes from source to destination
    for (size_t i = 0; i < num; ++i) {
        dest_ptr[i] = src_ptr[i];
    }

    return destination;
}
