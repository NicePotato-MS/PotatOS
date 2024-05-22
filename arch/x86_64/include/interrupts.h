#pragma once

#include <stdint.h>

#define INTERRUPT_DIVIDE_ERROR 0
#define INTERRUPT_DEBUG_EXCEPTION 1
#define INTERRUPT_NON_MASKABLE 2
#define INTERRUPT_BREAKPOINT 3
#define INTERRUPT_OVERFLOW 4
#define INTERRUPT_BOUND_RANGE_EXCEEDED 5
#define INTERRUPT_INVALID_OPCODE 6
#define INTERRUPT_NO_MATH_COPROCESSOR 7
#define INTERRUPT_DOUBLE_FAULT 8
#define INTERRUPT_COPROCESSOR_SEGMENT_OVERRUN 9
#define INTERRUPT_INVALID_TSS 10
#define INTERRUPT_SEGMENT_NOT_PRESENT 11
#define INTERRUPT_STACK_SEGMENT_FAULT 12
#define INTERRUPT_GENERAL_PROTECTION 13
#define INTERRUPT_PAGE_FAULT 14
#define INTERRUPT_MACHINE_FAULT 16
#define INTERRUPT_ALIGNMENT_CHECK 17
#define INTERRUPT_MACHINE_CHECK 18
#define INTERRUPT_SIMD_EXCEPTION 19
#define INTERRUPT_VIRTUALIZATION_EXCEPTION 20
#define INTERRUPT_CONTROL_PROTECTION_EXCEPTION 21



namespace idt {
    typedef struct {
        uint16_t isr_low;
        uint16_t gdt_cs;
        uint8_t ist; 
        uint8_t attributes;
        uint16_t isr_mid;
        uint32_t isr_high;
        uint32_t reserved;
    } __attribute__((packed)) isr_entry;

    extern isr_entry array[256];

    void SetEntry(uint8_t vector, void* isr, uint8_t ist, uint8_t flags);
    void Init();
}

