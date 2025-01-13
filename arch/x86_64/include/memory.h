#pragma once

#include <stdint.h>

#include <types.h>

#include <limine.h>
namespace memory {
    extern const char *memmap_type[];
    
    extern limine_memmap_response* memmap;
    extern size_t memmap_entry_count;
    extern limine_memmap_entry** memmap_entries;

    extern size_t hhdm;

    size_t PhysicalAddressToVirtual(size_t address);
    void Init();
}
