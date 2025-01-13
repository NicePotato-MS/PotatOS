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


size_t freelist_head = 0;
size_t freelist_end = 0;

size_t palloc() {
    if (!freelist_head) {
        return balloc();
    }

    if (freelist_head == freelist_end) {
        freelist_head = 0;
        return freelist_end;
    } else {
        size_t free_page = freelist_head;
        freelist_head = *(size_t*)freelist_head;
        return free_page;
    }
}

template <typename T>
void pfree(T address) {
    if (freelist_head != 0) {
        *(T*)freelist_end = address;
        freelist_end = reinterpret_cast<size_t>(address);
    } else {
        freelist_head = reinterpret_cast<size_t>(address);
        freelist_end = reinterpret_cast<size_t>(address);
    }
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

    // krnl::Printf_info("Memory Map");
    // for (size_t i = 0; i < memory::memmap->entry_count; i++) {
    //     limine_memmap_entry* entry = memory::memmap->entries[i];
    //     krnl::Printf("%0#6X : %22s : %018p - %018p\n",
    //         i,
    //         memmap_type[entry->type],
    //         entry->base,
    //         entry->base + entry->length - 1
    //     );
    // }

    krnl::Printf_info("Allocating pages");
    uint8_t* page1 = (uint8_t*)palloc();
    uint8_t* page2 = (uint8_t*)palloc();
    krnl::Printf_ok("Allocated Pages");
    krnl::Printf("Page 1: %18p\n", page1);
    krnl::Printf("Page 2: %18p\n", page2);
    krnl::Printf_info("Freeing pages");
    pfree(page1);
    pfree(page2);
    krnl::Printf_ok("Freed Pages");
    krnl::Printf_info("Freelist head: %18p", freelist_head);
    krnl::Printf_info("Freelist end:  %18p", freelist_end);
    krnl::Printf_info("Re-allocating pages");
    uint8_t* page3 = (uint8_t*)palloc();
    uint8_t* page4 = (uint8_t*)palloc();
    krnl::Printf_ok("Re-allocated Pages");
    krnl::Printf("Page 1: %18p\n", page3);
    krnl::Printf("Page 2: %18p\n", page4);
}