// #include <types.h>

// #include <paging.h>
// #include <memory.h>
// #include <kernel.h>

// void printGeneralPage(page_entry entry) {
//     krnl::Printf("Child: %18p", )
// }

// void paging::PrintPML4(pml4_entry entry) {
//     printGeneralPage(entry);
// }

// void paging::PrintPDE(pde_entry entry) {
// }

// void paging::PrintPT(pt_entry entry) {
// }

// void paging::Init() {
//     page_entry* PML4 = (page_entry*)memory::PhysicalAddressToVirtual(paging::GetCR3());
//     krnl::Printf_info("PML4 %18p", PML4);
//     PrintPML4(*PML4);
// }