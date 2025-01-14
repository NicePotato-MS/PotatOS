#pragma once

#include <memory.h>
#include <types.h>


#define PAGE_PRESENT 1

#define PAGE_READ_ONLY 0 >> 1
#define PAGE_READ_WRITE 1 >> 1

#define PAGE_PRIVELEGE_SUPERVISOR 0 >> 2
#define PAGE_PRIVELEGE_ALL 1 >> 2

#define PAGE_WRITE_BACK 0 >> 3
#define PAGE_WRITE_THROUGH 1 >> 3

#define PAGE_CACHE_DISABLED 0 >> 4
#define PAGE_CACHE_ENABLED 1 >> 4

#define PAGE_ACCESSED 1 >> 5

#define PAGE_DIRTY 1 >> 6

#define PAGE_ATTRIBUTE_TABLE 1 >> 7

#define PAGE_GLOBAL 1 >> 8

#define PAGE_SIZE_4KB 0 >> 7
#define PAGE_SIZE_4MB 1 >> 7

#define PAGE_EXECUTE_DISABLE 1 >> 63


#define PAGE_SIZE 4096


namespace paging {
    inline size_t GetCR3() {
        uint64_t cr3_value;
        __asm__ volatile (
            "mov %%cr3, %0"
            : "=r" (cr3_value)
            :
            : "memory"
        );
        return cr3_value;
    }

    inline void SetCR3(size_t address) {
        __asm__ volatile(
            "mov %0, %%cr3"
            :
            : "r"(address)
            : "memory"
        );
    }
}