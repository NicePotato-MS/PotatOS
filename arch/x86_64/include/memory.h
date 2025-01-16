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

    [[noreturn]] void OutOfMemory();

    size_t balloc();


    extern size_t freelist_head;
    extern size_t freelist_end;
    
    inline size_t palloc() {
        if (__builtin_expect(freelist_head == freelist_end, 0)) {
            OutOfMemory();
        } else {
            size_t free_page = freelist_head;
            freelist_head = *(size_t*)freelist_head;
            __builtin_prefetch((void*)freelist_head, 0, 1);
            return free_page;
        }
    }

    template <typename Address>
    inline void pfree(Address address) {
        *(Address*)freelist_end = address;
        freelist_end = reinterpret_cast<size_t>(address);
    }

    void Init();
}
