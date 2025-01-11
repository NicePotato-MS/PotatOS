#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <utils.h>

#include <serial.h>
#include <kernel.h>
#include <limine.h>
#include <ansi.h>
#include <arch.h>
#include <ps2.h>

#include <fonts/dina_7x16.h>

LIMINE_BASE_REVISION(2);



extern "C" void _kmain() {
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        krnl::Panic(KERNEL_PANIC_BOOTLOADER_UNSUPPORTED);
    }

    arch::Setup();

    ps2::Init();

    krnl::Printf("PotatOS Kernel Version %u.%u.%u-%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_DESCRIPTION);
    krnl::Printf("Powered by stupidity TM\n");

    while (true) {}
}
