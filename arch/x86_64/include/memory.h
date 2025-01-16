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

    template <typename Address>
    inline size_t PhysicalAddressToVirtual(Address address) {
        return reinterpret_cast<size_t>(address) + memory::hhdm;
    }

    template <typename Address>
    inline size_t VirtualAddressToPhysical(Address address) {
        return reinterpret_cast<size_t>(address) - memory::hhdm;
    }

    size_t balloc();
    void Init();
}
