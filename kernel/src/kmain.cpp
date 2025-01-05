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
#include <ps2.h>

#include <fonts/dina_7x16.h>

LIMINE_BASE_REVISION(2);

volatile struct limine_memmap_request memmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = NULL
};

const char *memmap_type[] = {
    "Usable",
    "Reserved",
    "ACPI Reclaimable",
    "ACPI NVS",
    "Bad Memory",
    "Bootloader Reclaimable",
    "Kernel / Modules",
    "Framebuffer"
};

extern "C" void _kmain() {
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        krnl::Panic(KERNEL_PANIC_BOOTLOADER_UNSUPPORTED);
    }

    arch::Setup();

    ps2::Init();

    #ifdef DEBUG
        krnl::Printf("PotatOS Kernel Version %u.%u.%u-debug\n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR,
            KERNEL_VERSION_PATCH);
    #else
        krnl::Printf("PotatOS Kernel Version %u.%u.%u\n", KERNEL_VERSION_MAJOR, KERNEL_VERSION_MINOR,
            KERNEL_VERSION_PATCH);
    #endif

    uint64_t cr3;

    __asm__ volatile (
        "mov %%cr3, %0"
        : "=r"(cr3)
    );

    krnl::Printf_info("Memory Map");

    if (!memmap_req.response) {
        krnl::Printf_error("No Memory Map!");
        krnl::Panic(KERNEL_PANIC_UNKNOWN_ERROR);
    }

    for (size_t i = 0; i < memmap_req.response->entry_count; i++) {
        limine_memmap_entry *entry = memmap_req.response->entries[i];
        krnl::Printf("%0#6X : %22s : %018p - %018p\n", i, memmap_type[entry->type], entry->base,
            entry->base + entry->length - 1);
    }


        
    while (true) {
        
    }
}
