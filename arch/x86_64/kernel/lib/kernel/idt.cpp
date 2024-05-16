#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <interrupts.h>
#include <kernel.h>
#include <gdt.h>

typedef struct {
    uint16_t isr_low;
    uint16_t gdt_cs;
    uint8_t ist; 
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry;

static idt_entry idt_array[256];

struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr;

__attribute__((interrupt)) static void default_handler(void *ptr) {
    krnl::Printf("Interrupt, argument passed: %p\n", ptr);
    krnl::Panic(KERNEL_PANIC_SUCCESS);
}

void idt::SetEntry(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry* entry = &idt_array[vector];

    entry->isr_low = (uint64_t)isr & 0xFFFF;
    entry->gdt_cs = GDT_CS_KERNEL;
    entry->ist = 0;
    entry->attributes = flags;
    entry->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    entry->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

void idt::Init() {
    idtr.base = (uintptr_t)&idt_array;
    idtr.limit = (uint16_t)sizeof(idt_entry) * 255;

    for (size_t vector = 0; vector < 256; vector++) {
        SetEntry(vector, reinterpret_cast<void*>(&default_handler), 0x8E);
    }

    __asm__ volatile("cli");
    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}