#pragma once

#include <stdint.h>

namespace idt {
    void SetEntry(uint8_t vector, void* isr, uint8_t flags);
    void Init();
}

