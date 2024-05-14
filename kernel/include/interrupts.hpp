#pragma once

#include <stdint.h>

void idt_SetEntry(uint8_t vector, void* isr, uint8_t flags);
void idt_Init();