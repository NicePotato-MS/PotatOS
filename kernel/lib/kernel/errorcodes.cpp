#include <stdint.h>

#include <kernel.h>

const char *category_misc[] = {
    "Operation completed successfully"
};

const char *category_memory[] = {
    "Page Fault"
};

const char **kernel_error_categories[] = {
    category_misc,
    category_memory
};

const char *kernel_error_category_names[] = {
    "Misc",
    "Memory"
};

const uint16_t kernel_error_category_sizes[] = {
    0,
    0
};