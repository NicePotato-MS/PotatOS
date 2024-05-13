#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <ansi.h>

void krn_Panic(uint32_t errorcode) {
    krn_Printk_panic("Kernel Panic!");
    krn_Printk("%s%s%s", ANSI_RESET, ANSI_RED, ANSI_BOLD);
    krn_Printk("Error code: %#010X\n", errorcode);
    uint16_t category = errorcode >> 16;
    uint16_t error = errorcode;
    if (category > KERNEL_ERROR_CATEGORIES ||
    error > kernel_error_category_sizes[category]) {
        krn_Printk("Category: ???\nError: ???\n");
    } else {
        krn_Printk("Category: %s\n", kernel_error_category_names[category]);
        krn_Printk("Error: %s\n", kernel_error_categories[category][error]);
    }
    halt();
}