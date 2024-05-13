#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <ansi.h>

void panic(uint32_t errorcode) {
    printk_panic("Kernel Panic!");
    printk("%s%s%s", ANSI_RESET, ANSI_RED, ANSI_BOLD);
    printk("Error code: %#010X\n", errorcode);
    uint8_t category = errorcode;
    uint8_t error = errorcode << 8;
    if (category > KERNEL_ERROR_CATEGORIES ||
    error > kernel_error_category_sizes[category]) {
        category = KERNEL_ERRORCATEGORY_MISC;
        error = KERNEL_ERROR_MISC_UNKNOWN;
    }
    printk("Category: %s\n", kernel_error_category_names[category]);
    printk("Error: %s\n", kernel_error_categories[category][error]);
    halt();
}