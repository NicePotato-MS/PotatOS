#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <ansi.h>

void krnl::Panic(uint32_t errorcode) {
    krnl::Printf_panic("Kernel Panic!");
    krnl::Printf("%s%s%s", ANSI_RESET, ANSI_RED, ANSI_BOLD);
    krnl::Printf("Error code: %#010X\n", errorcode);
    uint16_t category = errorcode >> 16;
    uint16_t error = errorcode;
    if (category > KERNEL_ERROR_CATEGORIES ||
    error > kernel_error_category_sizes[category]) {
        krnl::Printf("Category: ???\nError: ???\n");
    } else {
        krnl::Printf("Category: %s\n", kernel_error_category_names[category]);
        krnl::Printf("Error: %s\n", kernel_error_categories[category][error]);
    }
    halt();
}