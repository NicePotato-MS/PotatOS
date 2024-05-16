#pragma once

#include <stdint.h>
#include <stdarg.h>

#define KERNEL_VERSION_MAJOR 0
#define KERNEL_VERSION_MINOR 1
#define KERNEL_VERSION_PATCH 0


#define KERNEL_ERROR_CATEGORIES 2

#define KERNEL_ERRORCATEGORY_MISC 0x0000

#define KERNEL_ERROR_MISC_SUCCESS 0x0000

#define KERNEL_ERRORCATEGORY_MEMORY 0x0001

#define KERNEL_ERROR_MEMORY_PAGEFAULT

#define KERNEL_PANIC_SUCCESS 0x00000000
#define KERNEL_PANIC_PAGEFAULT 0x00010000

extern const char **kernel_error_categories[];
extern const char *kernel_error_category_names[];
extern const uint16_t kernel_error_category_sizes[];

void halt(void);

namespace krnl {
    void Panic(uint32_t errorcode);
    void Printf_varg(const char *str, va_list va);
    void Printf(const char *str, ...);
    void Printf_ok(const char *str, ...);
    void Printf_fail(const char *str, ...);
    void Printf_info(const char *str, ...);
    void Printf_warn(const char *str, ...);
    void Printf_error(const char *str, ...);
    void Printf_panic(const char *str, ...);
}