#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <interrupts.h>
#include <kernel.h>
#include <gdt.h>
#include <segment.h>


idt::isr_entry idt::array[256];

struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr;

__attribute__((interrupt)) static void default_handler(void* ptr) {
    krnl::Printf_error("Unhandled Interrupt at %p", ptr);
    krnl::Panic(KERNEL_PANIC_UNHANDLED_INTERRUPT);
}

__attribute__((interrupt)) static void division_error_handler(void* ptr) {
    krnl::Printf_error("Division by Zero at %p", ptr);
    krnl::Panic(KERNEL_PANIC_DIVISION_BY_ZERO);
}

__attribute__((interrupt)) static void invalid_opcode_handler(void* ptr) {
    krnl::Printf_error("Division by Zero at %p", ptr);
    krnl::Panic(KERNEL_PANIC_DIVISION_BY_ZERO);
}

__attribute__((interrupt)) static void stack_segment_fault_handler(void* ptr) {
    krnl::Printf_error("Segmentation Fault at %p", ptr);
    krnl::Panic(KERNEL_PANIC_SEGMENTATION_FAULT);
}

__attribute__((interrupt)) static void general_protection_handler(void* ptr) {
    krnl::Printf_error("General Protection Fault at %p", ptr);
    krnl::Panic(KERNEL_PANIC_GENERAL_PROTECTION_UNHANDLED);
}

__attribute__((interrupt)) static void page_fault_handler(void* ptr) {
    krnl::Printf_error("Page Fault at %p", ptr);
    krnl::Panic(KERNEL_PANIC_PAGE_FAULT);
}

void idt::SetEntry(uint8_t vector, void* isr, uint8_t ist, uint8_t flags) {
    idt::isr_entry* entry = &idt::array[vector];

    entry->isr_low = (uint64_t)isr & 0xFFFF;
    entry->gdt_cs = GDT_CS_KERNEL;
    entry->ist = ist;
    entry->attributes = flags;
    entry->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    entry->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    entry->reserved = 0;
}

#define NEW_TRAP(vector, isr) SetEntry(vector, reinterpret_cast<void*>(&isr), 0, \
SYS_SD_PRESENT | SYS_SD_RING_0 | SYS_SD_TYPE_TRAP);

void idt::Init() {
    idtr.base = (uintptr_t)&idt::array;
    idtr.limit = (uint16_t)sizeof(idt::isr_entry) * 255;

    for (size_t vector = 0; vector < 32; vector++) {
        NEW_TRAP(vector, default_handler)
    }

    NEW_TRAP(INTERRUPT_DIVIDE_ERROR, division_error_handler)
    NEW_TRAP(INTERRUPT_INVALID_OPCODE, invalid_opcode_handler)
    NEW_TRAP(INTERRUPT_STACK_SEGMENT_FAULT, stack_segment_fault_handler);
    NEW_TRAP(INTERRUPT_GENERAL_PROTECTION, general_protection_handler);
    NEW_TRAP(INTERRUPT_PAGE_FAULT, page_fault_handler);

    __asm__ volatile("cli");
    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}