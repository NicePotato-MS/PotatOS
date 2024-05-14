#include <kernel.hpp>

void halt(void) {
    __asm__("cli");
    while (1) { __asm__("hlt"); }
}