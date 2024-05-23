#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <ansi.h>

void krnl::Panic(uint32_t errorcode) {
    krnl::Printf_panic("Kernel Panic!");
    krnl::Printf("%s%s%s", ANSI_RESET, ANSI_RED, ANSI_BOLD);
    krnl::Printf("Error code: %s%s%#010X\n", ANSI_RESET, ANSI_BRIGHT_RED, errorcode);
    krnl::Printf("%s%s", ANSI_RED, ANSI_BOLD);
    uint16_t category = errorcode >> 16;
    uint16_t error = errorcode;
    if (category > KERNEL_ERROR_CATEGORIES ||
    error > kernel_error_category_sizes[category]) {
        krnl::Printf("Category: %s%s???\nError: ???\n", ANSI_RESET, ANSI_BRIGHT_RED);
    } else {
        krnl::Printf("Category: %s%s%s\n", ANSI_RESET, ANSI_BRIGHT_RED,
            kernel_error_category_names[category]);
        krnl::Printf("%s%s", ANSI_RED, ANSI_BOLD);
        krnl::Printf("Error: %s%s%s\n", ANSI_RESET, ANSI_BRIGHT_RED,
            kernel_error_categories[category][error]);
    }
    halt();
}