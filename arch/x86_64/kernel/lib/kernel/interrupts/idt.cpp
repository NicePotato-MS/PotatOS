#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <interrupts.hpp>
#include <kernel.hpp>

typedef struct {
    uint16_t isr_low;
    uint16_t gdt_cs;
    uint8_t ist; 
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry;

__attribute__((aligned(0x10))) static idt_entry idt[256];

struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr;

__attribute__((interrupt)) static void default_handler(void *ptr) {
    krn_Printk("Interrupt\n");
}

void idt_SetEntry(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry* entry = &idt[vector];

    entry->isr_low = (uint64_t)isr & 0xFFFF;
    entry->gdt_cs = 0x5;
    entry->ist = 0;
    entry->attributes = flags;
    entry->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    entry->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

void idt_Init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry) * 255;

    for (size_t vector = 0; vector < 32; vector++) {
        idt_SetEntry(vector, reinterpret_cast<void*>(default_handler), 0x8E);
    }

    __asm__ volatile("cli");
    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}