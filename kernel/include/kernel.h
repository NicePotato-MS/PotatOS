#pragma once

#include <stdint.h>
#include <stdarg.h>

#define KERNEL_VERSION_MAJOR 0
#define KERNEL_VERSION_MINOR 1
#define KERNEL_VERSION_PATCH 2


#define KERNEL_ERROR_CATEGORIES 4

#define KERNEL_PANIC_SUCCESS 0x00000000
#define KERNEL_PANIC_BOOTLOADER_UNSUPPORTED 0x00000001
#define KERNEL_PANIC_UNKNOWN_ERROR 0x00000002

#define KERNEL_PANIC_PAGE_FAULT 0x00010000
#define KERNEL_PANIC_SEGMENTATION_FAULT 0x00010001

#define KERNEL_PANIC_UNHANDLED_INTERRUPT 0x00020000
#define KERNEL_PANIC_GENERAL_PROTECTION_UNHANDLED 0x00020001

#define KERNEL_PANIC_DIVISION_BY_ZERO 0x00030000


extern const char **kernel_error_categories[];
extern const char *kernel_error_category_names[];
extern const uint16_t kernel_error_category_sizes[];

void halt(void);

namespace krnl {
    void Panic(uint32_t errorcode);
    void Printf(const char *str, va_list va);
    void Printf(const char *str, ...);
    void Printf_ok(const char *str, ...);
    void Printf_fail(const char *str, ...);
    void Printf_info(const char *str, ...);
    void Printf_warn(const char *str, ...);
    void Printf_error(const char *str, ...);
    void Printf_panic(const char *str, ...);
}