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

    krnl::Printf("PotatOS Kernel Version %u.%u.%u\n", KERNEL_VERSION_MAJOR,
        KERNEL_VERSION_MINOR, KERNEL_VERSION_PATCH);

    tty::tty1.Printf("ANSI test wow\n");
    tty::tty1.Printf("\e[31mRed \e[32mGreen \e[34mBlue\n\e[91mRed \e[92mGreen \e[94mBlue\n");
    tty::tty1.Printf("\e[1m");
    tty::tty1.Printf("\e[31mRed \e[32mGreen \e[34mBlue\n\e[91mRed \e[92mGreen \e[94mBlue\n");
    tty::tty1.Printf("\e[m\e[31mR\e[32ma\e[33mi\e[34mn\e[35mb\e[36mo\e[31mw\n");
    tty::tty1.Printf("\e[1m\e[31mR\e[32ma\e[33mi\e[34mn\e[35mb\e[36mo\e[31mw\n");
    tty::tty1.Printf("\e[m\e[31m\e[44mFoo \e[32m\e[41mBar \e[34m\e[42mBaz\n");
    tty::tty1.Printf("\e[1m\e[31m\e[44mFoo \e[32m\e[41mBar \e[34m\e[42mBaz\n\e[0m\n");

    while (true) {}
}
