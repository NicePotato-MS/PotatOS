#include <types.h>

#include <paging.h>
#include <memory.h>
#include <kernel.h>

#include <ansi.h>

page_table::L4 paging::KernelL4 = NULL;

size_t L3Count;
size_t L2Count;
size_t L1Count;
size_t pageCount;

void countL1(page_table::L1 L1) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::L1Entry entry = L1[i];
        if (entry.getPresent()) {
            pageCount++;
        }
    }
}

void recurseCopyL2(page_table::L2 L2) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::L2Entry entry = L2[i];
        if (entry.getPresent()) {
            L1Count++;
            page_table::L1 L1 = entry.Fetch().CopyTo(memory::balloc());
            countL1(L1);
        }
    }
}

void recurseCopyL3(page_table::L3 L3) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::L3Entry entry = L3[i];
        if (entry.getPresent()) {
            L2Count++;
            page_table::L2 L2 = entry.Fetch().CopyTo(memory::balloc());
            recurseCopyL2(L2);
        }
    }
}

void recurseCopyL4(page_table::L4 L4) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::L4Entry entry = L4[i];
        if (entry.getPresent()) {
            L3Count++;
            page_table::L3 L3 = entry.Fetch().CopyTo(memory::balloc());
            recurseCopyL3(L3);
        }
    }
}

void paging::Init() {
    page_table::L4 sourceL4(memory::PhysicalAddressToVirtual(paging::GetCR3()));
    krnl::Printf_info("Moving Page Tables from L4 %18p", sourceL4.entries);
    paging::KernelL4 = sourceL4.CopyTo(memory::balloc());
    krnl::Printf_info("New L4 at %18p", paging::KernelL4.entries);

    L3Count = 0;
    L2Count = 0;
    L1Count = 0;
    pageCount = 0;

    recurseCopyL4(paging::KernelL4);

    krnl::Printf_ok("Copied page tables for %zu pages", pageCount);
    krnl::Printf(ANSI_BRIGHT_GREEN);
    krnl::Printf("        L3 : %zu\n", L3Count);
    krnl::Printf("        L2 : %zu\n", L2Count);
    krnl::Printf("        L1 : %zu\n", L1Count);
    krnl::Printf(ANSI_RESET);

    paging::SetCR3(memory::VirtualAddressToPhysical(paging::KernelL4.entries));
    krnl::Printf_ok("Switched to new page tables");
}