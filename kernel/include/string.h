#pragma once

#include <stddef.h>

inline void* memcpy(void* destination, const void* source, size_t num) {
    char* dest_ptr = (char*)destination;
    const char* src_ptr = (const char*)source;

    for (size_t i = 0; i < num; ++i) {
        dest_ptr[i] = src_ptr[i];
    }

    return destination;
}

inline void* memmove(void* dest, const void* src, size_t n) {
    char* dest_ptr = (char*)dest;
    const char* src_ptr = (const char*)src;

    if (src_ptr < dest_ptr) {
        for (size_t i = n; i > 0; --i) {
            dest_ptr[i - 1] = src_ptr[i - 1];
        }
    } else if (src_ptr > dest_ptr) {
        for (size_t i = 0; i < n; ++i) {
            dest_ptr[i] = src_ptr[i];
        }
    }

    return dest;
}

inline void* memset(void* dest, int value, size_t n) {
    unsigned char* dest_ptr = (unsigned char*)dest;

    for (size_t i = 0; i < n; ++i) {
        dest_ptr[i] = (unsigned char)value;
    }

    return dest;
}

inline int memcmp(const void* ptr1, const void* ptr2, size_t n) {
    const unsigned char *byte_ptr1 = (const unsigned char*)ptr1;
    const unsigned char *byte_ptr2 = (const unsigned char*)ptr2;

    for (size_t i = 0; i < n; ++i) {
        if (byte_ptr1[i] < byte_ptr2[i]) {
            return -1;
        } else if (byte_ptr1[i] > byte_ptr2[i]) {
            return 1;
        }
    }

    return 0;
}

inline size_t strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s);
    return (s - str);
}