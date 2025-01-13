#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <types.h>

#include <kernel.h>
#include <limine.h>
#include <memory.h>
#include <paging.h>

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = NULL
};
limine_memmap_response* memory::memmap;
size_t memory::memmap_entry_count;
limine_memmap_entry** memory::memmap_entries;

volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = NULL
};

size_t memory::hhdm;

#define MEMMAP_USABLE 0
#define MEMMAP_RESERVED 1
#define MEMMAP_ACPI_RECLAIMABLE 2
#define MEMMAP_ACPI_NVS 3
#define MEMMAP_BAD 4
#define MEMMAP_BOOTLOADER 5
#define MEMMAP_KERNEL 6
#define MEMMAP_FRAMEBUFFER 7

const char *memory::memmap_type[] = {
    "Usable",
    "Reserved",
    "ACPI Reclaimable",
    "ACPI NVS",
    "Bad Memory",
    "Bootloader Reclaimable",
    "Kernel / Modules",
    "Framebuffer"
};

size_t memory::PhysicalAddressToVirtual(size_t address) {
    return address + memory::hhdm;
}


size_t bump_head = 0;
size_t bump_end = 0;
size_t bump_memmap_entry = 0;

size_t freelist_head;
size_t freelist_end;

void bumpGetNextRegion() {
    limine_memmap_entry* entry;

    while (bump_memmap_entry < memory::memmap_entry_count) {
        entry = memory::memmap_entries[bump_memmap_entry];
        switch (entry->type) {
            case MEMMAP_USABLE:
            case MEMMAP_BOOTLOADER:
            case MEMMAP_ACPI_RECLAIMABLE:
                bump_head = memory::PhysicalAddressToVirtual((entry->base + 0xFFF) & ~0xFFF) - PAGE_SIZE;
                bump_end = memory::PhysicalAddressToVirtual((entry->base + entry->length) & ~0xFFF) - PAGE_SIZE;
                if (bump_end - bump_head >= PAGE_SIZE) {
                    krnl::Printf_info("BA: %0#6X : %22s : %018p - %018p",
                        bump_memmap_entry,
                        memory::memmap_type[entry->type],
                        entry->base,
                        entry->base + entry->length - 1
                    );
                    bump_memmap_entry++;
                    
                    return;
                }
        }
        bump_memmap_entry++;
    }

    krnl::Panic(KERNEL_PANIC_OUT_OF_MEMORY);
}

size_t balloc() {
    if (bump_head == bump_end) {
        bumpGetNextRegion();
    }

    bump_head += PAGE_SIZE;
    return bump_head;
}




void memory::Init() {
    if (!memmap_request.response) {
        krnl::Printf_error("No Memory Map!");
        krnl::Panic(KERNEL_PANIC_BOOTLOADER_NO_MEMMAP);
    }

    memory::memmap = memmap_request.response;
    memory::memmap_entry_count = memory::memmap->entry_count;
    memory::memmap_entries = memory::memmap->entries;

    memory::hhdm = hhdm_request.response->offset;

    krnl::Printf_info("HHDM: %018p", hhdm);

    krnl::Printf_info("Memory Map");
    for (size_t i = 0; i < memory::memmap->entry_count; i++) {
        limine_memmap_entry* entry = memory::memmap->entries[i];
        krnl::Printf("%0#6X : %22s : %018p - %018p\n",
            i,
            memmap_type[entry->type],
            entry->base,
            entry->base + entry->length - 1
        );
    }
}