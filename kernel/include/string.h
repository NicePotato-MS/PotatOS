#pragma once

#include <stddef.h>

/* Copy n bytes from src to dest */
void* memcpy(void* dest, const void* src, size_t n);
/* Copy n bytes from src to dest, handling overlapping regions */
void *memmove(void *dest, const void *src, size_t n);
/* Set the first n bytes of the memory area pointed to by dest to the specified value */
void *memset(void *dest, int value, size_t n);
/* Compare the first n bytes of memory areas pointed to by ptr1 and ptr2 */
int memcmp(const void *ptr1, const void *ptr2, size_t n);

inline size_t strlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
    return (s - str);
}