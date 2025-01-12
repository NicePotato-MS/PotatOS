#pragma once

namespace memory {
    extern const char *memmap_type[];
    extern void* hhdm;

    void* PhysicalAddressToVirtual(void* address);
    void Init();
}
