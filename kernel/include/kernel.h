#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define KERNEL_ERROR_CATEGORIES 2

#define KERNEL_ERRORCATEGORY_MISC 0x0000

#define KERNEL_ERROR_MISC_UNKNOWN 0x0000

#define KERNEL_ERRORCATEGORY_MEMORY 0x0001

#define KERNEL_ERROR_MEMORY_PAGEFAULT

#define KERNEL_PANIC_UNKNOWN 0x00000000
#define KERNEL_PANIC_PAGEFAULT 0x00010000

extern const char **kernel_error_categories[];
extern const char *kernel_error_category_names[];
extern const uint16_t kernel_error_category_sizes[];

void halt(void);
void panic(uint32_t errorcode);

void printk(const char *str, ...);
void printk_ok(const char *str, ...);
void printk_fail(const char *str, ...);
void printk_info(const char *str, ...);
void printk_warn(const char *str, ...);
void printk_error(const char *str, ...);
void printk_panic(const char *str, ...);

#ifdef __cplusplus
}
#endif