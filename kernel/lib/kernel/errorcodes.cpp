#include <kernel.h>
#include <stdint.h>

const char *category_misc[] = {
    "Operation completed successfully",
    "Bootloader Unsupported (Make sure to use limine)",
    "Unknown Error"
};

const char *category_memory[] = {
    "Unhandled Page Fault",
    "Unhandled Segmentation Fault"
};

const char *category_interrupt[] = {
    "Unhandled Interrupt",
    "Unhandled General Protection"
};

const char *category_math[] = {
    "Unhandled Division by Zero"
};

const char *category_execution[] = {
    "Unhandled Invalid Opcode"
};

const char **kernel_error_categories[] = {category_misc, category_memory,
    category_interrupt, category_math, category_execution};

const char *kernel_error_category_names[] = {
    "Misc",
    "Memory",
    "Interrupt",
    "Math",
    "Execution"
};


#define SIZEOF_PTR_ARR(x) sizeof(x) / sizeof(uintptr_t)

const uint16_t kernel_error_category_sizes[] = {
    SIZEOF_PTR_ARR(category_misc) - 1, SIZEOF_PTR_ARR(category_memory) - 1,
    SIZEOF_PTR_ARR(category_interrupt) - 1, SIZEOF_PTR_ARR(category_math) - 1,
    SIZEOF_PTR_ARR(category_execution) - 1
};