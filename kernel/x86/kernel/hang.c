#include <kernel.h>

void halt(void) {
    asm("cli");
    while (1) {
        asm("hlt");
    }
}