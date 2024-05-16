#include <stdint.h>

#include <kernel.h>
#include <interrupts.h>
#include <gdt.h>
#include <arch.h>

void arch::Setup() {
    gdt::Load();
    gdt::SetSegments(GDT_CS_KERNEL, GDT_DS_KERNEL); // 64-bit code, 64-bit data
    krnl::Printf_ok("GDT Loaded");
    idt::Init();
    krnl::Printf_ok("IDT Initialized");
}