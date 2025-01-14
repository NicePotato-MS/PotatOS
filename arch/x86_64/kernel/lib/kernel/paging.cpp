#include <types.h>

#include <paging.h>
#include <memory.h>
#include <kernel.h>


void paging::Init() {
    page_table::PML4 PML4(memory::PhysicalAddressToVirtual(paging::getCR3()));
    krnl::Printf_info("PML4 %18p", PML4);
    PML4[0].Print();
}