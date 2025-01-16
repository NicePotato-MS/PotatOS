#include <types.h>

#include <paging.h>
#include <memory.h>
#include <kernel.h>

#include <ansi.h>

page_table::PML4 paging::KernelPML4 = NULL;

size_t PDPTCount;
size_t PDCount;
size_t PTCount;
size_t pageCount;

void countPT(page_table::PT PT) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::PTEntry entry = PT[i];
        if (entry.getPresent()) {
            pageCount++;
        }
    }
}

void recurseCopyPD(page_table::PD PD) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::PDEntry entry = PD[i];
        if (entry.getPresent()) {
            PTCount++;
            page_table::PT PT = entry.Fetch().CopyTo(memory::balloc());
            countPT(PT);
        }
    }
}

void recurseCopyPDPT(page_table::PDPT PDPT) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::PDPTEntry entry = PDPT[i];
        if (entry.getPresent()) {
            PDCount++;
            page_table::PD PD = entry.Fetch().CopyTo(memory::balloc());
            recurseCopyPD(PD);
        }
    }
}

void recurseCopyPML4(page_table::PML4 PML4) {
    for (size_t i = 0; i < page_table::ENTRY_COUNT; i++) {
        page_table::PML4Entry entry = PML4[i];
        if (entry.getPresent()) {
            PDPTCount++;
            page_table::PDPT PDPT = entry.Fetch().CopyTo(memory::balloc());
            recurseCopyPDPT(PDPT);
        }
    }
}

void paging::Init() {
    page_table::PML4 sourcePML4(memory::PhysicalAddressToVirtual(paging::GetCR3()));
    krnl::Printf_info("Moving Page Tables from PML4 %18p", sourcePML4.entries);
    paging::KernelPML4 = sourcePML4.CopyTo(memory::balloc());
    krnl::Printf_info("New PML4 at %18p", paging::KernelPML4.entries);

    PDPTCount = 0;
    PDCount = 0;
    PTCount = 0;
    pageCount = 0;

    recurseCopyPML4(paging::KernelPML4);

    krnl::Printf_ok("Copied page tables for %zu pages", pageCount);
    krnl::Printf(ANSI_BRIGHT_GREEN);
    krnl::Printf("        PDPT : %zu\n", PDPTCount);
    krnl::Printf("        PD   : %zu\n", PDCount);
    krnl::Printf("        PT   : %zu\n", PTCount);
    krnl::Printf(ANSI_RESET);

    paging::SetCR3(memory::VirtualAddressToPhysical(paging::KernelPML4.entries));
    krnl::Printf_ok("Switched to new page tables");
}