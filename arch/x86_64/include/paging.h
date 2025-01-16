// Most of the constexpr in this file will do nothing
// however for statically compiled entries it should speed things up
// (correct me if I'm wrong)

// Still new to many new C++ features so I probably did this all wrong

#pragma once

#include <string.h>

#include <types.h>

#include <memory.h>
#include <kernel.h>

namespace page {
    enum {
        SIZE = 4096,
    };
}
namespace page_table {
    enum {
        ENTRY_COUNT = 512,
        ADDRESS_MASK = 0xFFFFFFFFFFFFF000,
        CANONICAL_MASK = 0x0000FFFFFFFFFFFF,
        INDEX_MASK = 0x1FF
    };
    namespace shift {
        enum {
            PRESENT = 0,
            READ_WRITE = 1,
            PRIVELEGE = 2,
            WRITE_MODE = 3,
            CACHE_MODE = 4,
            ACCESSED = 5,

            SIZE = 7,

            DIRTY = 6,
            PAT = 7,
            GLOBAL = 8
        };

        enum {
            L4Index = 12 + 27,
            L3Index = 12 + 18,
            L2Index = 12 + 9,
            L1Index = 12
        };
    }

    namespace flag {
        enum {
            PRESENT = 1 >> shift::PRESENT,

            READ_WRITE = 1 >> shift::READ_WRITE,
            READ_ONLY = 0 >> shift::READ_WRITE,

            PRIVELEGE_ALL = 1 >> shift::PRIVELEGE,
            PRIVELEGE_KERNEL = 0 >> shift::PRIVELEGE,

            WRITE_THROUGH = 1 >> shift::WRITE_MODE,
            WRITE_BACK = 0 >> shift::WRITE_MODE,

            CACHE_DISABLE = 1 >> shift::CACHE_MODE,
            CACHE_ENABLE = 0 >> shift::CACHE_MODE,

            SIZE_4MB = 1 >> shift::SIZE,
            SIZE_4KB = 0 >> shift::SIZE,
        };
    }

    enum {
        KERNEL_READ_ONLY = flag::PRESENT | flag::READ_ONLY | flag::PRIVELEGE_KERNEL | flag::WRITE_BACK | flag::CACHE_ENABLE,
        KERNEL_READ_WRITE = flag::PRESENT | flag::READ_WRITE | flag::PRIVELEGE_KERNEL | flag::WRITE_BACK | flag::CACHE_ENABLE,
    };

    // Forward Declarations
    class L3;
    class L2;
    class L1;


    class PageTableEntry {
       public:
        size_t entry;
        inline constexpr PageTableEntry(size_t value) : entry(value) {}

        inline constexpr bool getPresent() const { return (entry >> shift::PRESENT) & 1; }
        inline constexpr bool getReadWrite() const { return (entry >> shift::READ_WRITE) & 1; }
        inline constexpr bool getPrivilege() const { return (entry >> shift::PRIVELEGE) & 1; }
        inline constexpr bool getWriteMode() const { return (entry >> shift::WRITE_MODE) & 1; }
        inline constexpr bool getCacheMode() const { return (entry >> shift::CACHE_MODE) & 1; }
        inline constexpr bool getAccessed() const { return (entry >> shift::ACCESSED) & 1; }

        inline constexpr size_t getPhysicalAddress() const { return entry & ADDRESS_MASK; }
        inline size_t getVirtualAddress() const { return memory::PhysicalAddressToVirtual(getPhysicalAddress()); }

        void PrintBase() const {
            krnl::Printf("Address (V) : %18p\n", getVirtualAddress());
            krnl::Printf("Present     : %s\n", getPresent() ? "Yes" : "No");
            krnl::Printf("RW Access   : %s\n", getReadWrite() ? "ReadWrite" : "ReadOnly");
            krnl::Printf("Privilege   : %s\n", getPrivilege() ? "KernelUser" : "KernelOnly");
            krnl::Printf("Write Mode  : %s\n", getWriteMode() ? "WriteThrough" : "WriteBack");
            krnl::Printf("Cache Mode  : %s\n", getCacheMode() ? "Disabled" : "Enabled");
            krnl::Printf("Accessed    : %s\n", getAccessed() ? "Yes" : "No");
        }
    };

    class L4Entry : public PageTableEntry {
       public:
        inline constexpr L4Entry(size_t value) : PageTableEntry(value) {}
        void Print() const {
            krnl::Printf_info("L4 Entry");
            PrintBase();
        }

        inline L3 Fetch();
    };

    class L3Entry : public PageTableEntry {
       public:
        inline constexpr L3Entry(size_t value) : PageTableEntry(value) {}
        inline bool getAccessed() const { return (entry >> shift::ACCESSED) & 1; }

        void Print() const {
            krnl::Printf_info("L3 Entry");
            PrintBase();
        }

        inline L2 Fetch();
    };

    class L2Entry : public PageTableEntry {
       public:
        inline constexpr L2Entry(size_t value) : PageTableEntry(value) {}
        void Print() const {
            krnl::Printf_info("L2 Entry");
            PrintBase();
        }

        inline L1 Fetch();
    };

    class L1Entry : public PageTableEntry {
       public:
        inline constexpr L1Entry(size_t value) : PageTableEntry(value) {}
        void Print() const {
            krnl::Printf_info("L1 Entry");
            PrintBase();
        }
    };


    template <typename EntryType>
    class PageTable {
       protected:
        template <typename Address>
        inline PageTable BaseCopyTo(Address address) {
            memcpy(reinterpret_cast<void*>(address), entries, page::SIZE);
            return PageTable(address);
        }

       public:
        EntryType* entries;

        template <typename Address>
        inline PageTable(Address address) : entries(reinterpret_cast<EntryType*>(address)) {}
        
        inline EntryType& operator[](size_t index) { return entries[index]; }
        inline const EntryType& operator[](size_t index) const { return entries[index]; }

        inline void FillEntries(EntryType entry) {
            for (size_t i = 0; i < ENTRY_COUNT; i++) {
                entries[i] = entry;
            }
        }
    };

    class L4 : public PageTable<L4Entry> {
       public:
        template <typename Address>
        inline constexpr L4(Address address) : PageTable(address) {}

        template <typename Address>
        inline L4 CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };

    class L3 : public PageTable<L3Entry> {
       public:
        template <typename Address>
        inline constexpr L3(Address address) : PageTable(address) {}

        template <typename Address>
        inline L3 CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };

    class L2 : public PageTable<L2Entry> {
       public:
        template <typename Address>
        inline constexpr L2(Address address) : PageTable(address) {}

        template <typename Address>
        inline L2 CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };

    class L1 : public PageTable<L1Entry> {
       public:
        template <typename Address>
        inline constexpr L1(Address address) : PageTable(address) {}

        template <typename Address>
        inline L1 CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };


    inline L3 L4Entry::Fetch() {
        return L3(getVirtualAddress());
    }

    inline L2 L3Entry::Fetch() {
        return L2(getVirtualAddress());
    }

    inline L1 L2Entry::Fetch() {
        return L1(getVirtualAddress());
    }


    inline L4 NewL4Bump() {
        L4 newL4 = L4(memory::balloc());
        newL4.FillEntries(0);
        return newL4;
    }

    inline L3 NewL3Bump() {
        L3 newL3 = L3(memory::balloc());
        newL3.FillEntries(0);
        return newL3;
    }

    inline L2 NewL2Bump() {
        L2 newL2 = L2(memory::balloc());
        newL2.FillEntries(0);
        return newL2;
    }

    inline L1 NewL1Bump() {
        L1 newL1 = L1(memory::balloc());
        newL1.FillEntries(0);
        return newL1;
    }

    inline L4 NewL4() {
        L4 newL4 = L4(memory::palloc());
        newL4.FillEntries(0);
        return newL4;
    }

    inline L3 NewL3() {
        L3 newL3 = L3(memory::palloc());
        newL3.FillEntries(0);
        return newL3;
    }

    inline L2 NewL2() {
        L2 newL2 = L2(memory::palloc());
        newL2.FillEntries(0);
        return newL2;
    }

    inline L1 NewL1() {
        L1 newL1 = L1(memory::palloc());
        newL1.FillEntries(0);
        return newL1;
    }


    struct Indexes {
        size_t L4Index;
        size_t L3Index;
        size_t L2Index;
        size_t L1Index;
    };

    inline Indexes GetIndexes(size_t virtualAddress) {
        virtualAddress &= CANONICAL_MASK;
        size_t L4Index = (virtualAddress >> shift::L4Index) & INDEX_MASK;
        size_t L3Index = (virtualAddress >> shift::L3Index) & INDEX_MASK;
        size_t L2Index = (virtualAddress >> shift::L2Index) & INDEX_MASK;
        size_t L1Index = (virtualAddress >> shift::L1Index) & INDEX_MASK;
        return {L4Index, L3Index, L2Index, L1Index};
    }
}

namespace paging {
    extern page_table::L4 kernelL4;

    inline size_t GetCR3() {
        size_t cr3_value;
        __asm__ volatile (
            "mov %%cr3, %0"
            : "=r" (cr3_value)
            :
            : "memory"
        );
        return cr3_value;
    }

    template <typename Address>
    inline void SetCR3(Address address) {
        __asm__ volatile(
            "mov %0, %%cr3"
            :
            : "r"(address)
            : "memory"
        );
    }

    template <typename Address>
    void MapPagesBump(page_table::L4 L4, Address virtualAddress, Address physicalAddress);
    
    void Init();
}