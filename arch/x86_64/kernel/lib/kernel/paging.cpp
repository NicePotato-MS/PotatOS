#include <types.h>

#include <paging.h>
#include <memory.h>
#include <kernel.h>

#include <ansi.h>

extern size_t linker_KernelStart;
extern size_t linker_KernelWriteStart;
extern size_t linker_KernelEnd;


page_table::L4 paging::kernelL4 = NULL;

template <typename Address>
void paging::MapPagesBump(page_table::L4 L4, Address virtualAddress, Address physicalAddress) {
    page_table::Indexes indexes = page_table::GetIndexes(virtualAddress);
}

void paging::Init() {
    paging::kernelL4 = page_table::NewL4Bump();


    // paging::SetCR3(memory::VirtualAddressToPhysical(paging::kernelL4.entries));
    // krnl::Printf_ok("Switched to new page tables");
}