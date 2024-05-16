#pragma once

#include <stddef.h>

#define GDT_CS_KERNEL 5 << 3
#define GDT_DS_KERNEL 4 << 3

namespace gdt {
    void Load();
    void SetSegments(size_t code_segment, size_t data_segment);
}