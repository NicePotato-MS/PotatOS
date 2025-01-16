#include <types.h>

#include <paging.h>
#include <memory.h>
#include <kernel.h>

#include <ansi.h>

page_table::L4 paging::kernelL4 = NULL;

size_t L3Count;
size_t L2Count;
size_t L1Count;
size_t pageCount;

void paging::Init() {


    L3Count = 0;
    L2Count = 0;
    L1Count = 0;
    pageCount = 0;

    

    // krnl::Printf_ok("Copied page tables for %zu pages", pageCount);
    // krnl::Printf(ANSI_BRIGHT_GREEN);
    // krnl::Printf("        L3 : %zu\n", L3Count);
    // krnl::Printf("        L2 : %zu\n", L2Count);
    // krnl::Printf("        L1 : %zu\n", L1Count);
    // krnl::Printf(ANSI_RESET);

    // paging::SetCR3(memory::VirtualAddressToPhysical(paging::kernelL4.entries));
    // krnl::Printf_ok("Switched to new page tables");
}