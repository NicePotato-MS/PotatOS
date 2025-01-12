#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <memory.h>
#include <kernel.h>
#include <limine.h>

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = NULL
};

volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = NULL
};

void* memory::hhdm;

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

void* memory::PhysicalAddressToVirtual(void* address) {
    return (void*)((size_t)(address) + (size_t)(memory::hhdm));
}

void memory::Init() {
    if (!memmap_request.response) {
        krnl::Printf_error("No Memory Map!");
        krnl::Panic(KERNEL_PANIC_BOOTLOADER_NO_MEMMAP);
    }

    memory::hhdm = (void *)hhdm_request.response->offset;
    krnl::Printf_info("HHDM: %018p", hhdm);

    krnl::Printf_info("Memory Map");
    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
        limine_memmap_entry* entry = memmap_request.response->entries[i];
        krnl::Printf("%0#6X : %22s : %018p - %018p\n",
            i,
            memmap_type[entry->type],
            entry->base,
            entry->base + entry->length - 1
        );
    }


}