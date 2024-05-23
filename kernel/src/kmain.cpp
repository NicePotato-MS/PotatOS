#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <utils.h>

#include <serial.h>
#include <kernel.h>
#include <interrupts.h>
#include <limine.h>
#include <ansi.h>
#include <arch.h>
#include <gdt.h>
#include <tty.h>

#include <fonts/dina_7x16.h>

LIMINE_BASE_REVISION(2);

extern "C" void _kmain() {
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        krnl::Panic(KERNEL_PANIC_BOOTLOADER_UNSUPPORTED);
    }

    arch::Setup();

    krnl::Printf("PotatOS Kernel Version %u.%u.%u\n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR,
        KERNEL_VERSION_PATCH);

    while (true) {
        
    }
}
