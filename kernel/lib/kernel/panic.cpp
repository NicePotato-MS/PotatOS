#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <ansi.h>
#include <errorcodes.h>

void krnl::Panic(uint32_t errorcode) {
    krnl::Printf_panic("Kernel Panic!");
    krnl::Printf("%s%s%s", ansi::reset, ansi::fg::red, ansi::bold);
    krnl::Printf("Error code: %s%s%#010X\n", ansi::reset, ansi::fg::bright_red, errorcode);
    krnl::Printf("%s%s", ansi::fg::red, ansi::bold);
    uint16_t category = errorcode >> 16;
    uint16_t error = errorcode;
    if (category > errorcode::catergoryCount || error > errorcode::categorySizes[category]) {
        krnl::Printf("Category: %s%s???\nError: ???\n", ansi::reset, ansi::fg::bright_red);
    } else {
        krnl::Printf("Category: %s%s%s\n", ansi::reset, ansi::fg::bright_red, errorcode::categoryNames[category]);
        krnl::Printf("%s%s", ansi::fg::red, ansi::bold);
        krnl::Printf("Error: %s%s%s\n", ansi::reset, ansi::fg::bright_red, errorcode::categories[category][error]);
    }
    krnl::Printf("%s", ansi::reset);
    halt();
}