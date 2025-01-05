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
#define INTERRUPT_PIT 32
#define INTERRUPT_PS2_1 33
#define INTERRUPT_COM2 35
#define INTERRUPT_COM1 36
#define INTERRUPT_LPT2 37
#define INTERRUPT_FLOPPY_DISK 38
#define INTERRUPT_LPT1 39
#define INTERRUPT_SPURIOUS 40
#define INTERRUPT_CMOS_RTC 41
#define INTERRUPT_PS2_2 45
#define INTERRUPT_COPROCESSOR 46
#define INTERRUPT_ATA_HDD_1 47
#define INTERRUPT_ATA_HDD_2 48

#define INTERRUPT_MASK_PIT 0
#define INTERRUPT_MASK_PS2_1 1
#define INTERRUPT_MASK_COM2 3
#define INTERRUPT_MASK_COM1 4
#define INTERRUPT_MASK_LPT2 5
#define INTERRUPT_MASK_FLOPPY_DISK 6
#define INTERRUPT_MASK_LPT1 7
#define INTERRUPT_MASK_SPURIOUS 8
#define INTERRUPT_MASK_CMOS_RTC 9
#define INTERRUPT_MASK_PS2_2 12
#define INTERRUPT_MASK_COPROCESSOR 13
#define INTERRUPT_MASK_ATA_HDD_1 14
#define INTERRUPT_MASK_ATA_HDD_2 15

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

#define NEW_TRAP(vector, isr)                          \
    idt::SetEntry(vector, reinterpret_cast<void*>(&isr), 0, \
        SYS_SD_PRESENT | SYS_SD_RING_0 | SYS_SD_TYPE_TRAP);

#define NEW_INTERRUPT(vector, isr)                          \
    idt::SetEntry(vector, reinterpret_cast<void*>(&isr), 0, \
        SYS_SD_PRESENT | SYS_SD_RING_0 | SYS_SD_TYPE_INTERRUPT);
