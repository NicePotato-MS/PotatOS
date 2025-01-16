#include <stdint.h>

#include <errorcodes.h>

#include <utils.h>

const char *category_misc[] = {
    "Operation completed successfully",
    "Bootloader Unsupported (Make sure to use limine)",
    "Unknown Error"
};

const char *category_memory[] = {
    "Unhandled Page Fault",
    "Unhandled Segmentation Fault",
    "Bootloader did not pass memory map",
    "Out of Physical Memory",
    "Mapping out of Virtual Memory"
};

const char *category_interrupt[] = {
    "Unhandled Interrupt",
    "Unhandled General Protection Fault"
};

const char *category_math[] = {
    "Unhandled Division by Zero"
};

const char *category_execution[] = {
    "Unhandled Invalid Opcode"
};

const char **errorcode::categories[] = {
    category_misc,
    category_memory,
    category_interrupt,
    category_math,
    category_execution
};

const char *errorcode::categoryNames[] = {
    "Misc",
    "Memory",
    "Interrupt",
    "Math",
    "Execution"
};


const uint16_t errorcode::catergoryCount = sizeof(errorcode::categories);

const uint16_t errorcode::categorySizes[] = {
    SIZEOF_PTR_ARR(category_misc) - 1,
    SIZEOF_PTR_ARR(category_memory) - 1,
    SIZEOF_PTR_ARR(category_interrupt) - 1,
    SIZEOF_PTR_ARR(category_math) - 1,
    SIZEOF_PTR_ARR(category_execution) - 1
};