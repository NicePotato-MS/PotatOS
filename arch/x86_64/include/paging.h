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
        ADDRESS_MASK = 0xFFFFFFFFFFFFF000
    };
    namespace shift {
        enum {
            PRESENT = 0,
            READ_WRITE = 1,
            PRIVELEGE = 2,
            WRITE_THROUGH = 3,
            CACHE_DISABLE = 4,
            ACCESSED = 5,

            SIZE = 7,

            DIRTY = 6,
            PAT = 7,
            GLOBAL = 8
        };
    }

    // Forward Declarations
    class PDPT;
    class PD;
    class PT;


    class PageTableEntry {
       public:
        size_t entry;
        inline constexpr PageTableEntry(size_t value) : entry(value) {}

        inline constexpr bool getPresent() const { return (entry >> shift::PRESENT) & 1; }
        inline constexpr bool getReadWrite() const { return (entry >> shift::READ_WRITE) & 1; }
        inline constexpr bool getPrivilege() const { return (entry >> shift::PRIVELEGE) & 1; }
        inline constexpr bool getWriteMode() const { return (entry >> shift::WRITE_THROUGH) & 1; }
        inline constexpr bool getCacheMode() const { return (entry >> shift::CACHE_DISABLE) & 1; }
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

    class PML4Entry : public PageTableEntry {
       public:
        inline constexpr PML4Entry(size_t value) : PageTableEntry(value) {}
        void Print() const {
            krnl::Printf_info("PML4 Entry");
            PrintBase();
        }

        inline PDPT Fetch();
    };

    class PDPTEntry : public PageTableEntry {
       public:
        inline constexpr PDPTEntry(size_t value) : PageTableEntry(value) {}
        inline bool getAccessed() const { return (entry >> shift::ACCESSED) & 1; }

        void Print() const {
            krnl::Printf_info("PDPT Entry");
            PrintBase();
        }

        inline PD Fetch();
    };

    class PDEntry : public PageTableEntry {
       public:
        inline constexpr PDEntry(size_t value) : PageTableEntry(value) {}
        void Print() const {
            krnl::Printf_info("PD Entry");
            PrintBase();
        }

        inline PT Fetch();
    };

    class PTEntry : public PageTableEntry {
       public:
        inline constexpr PTEntry(size_t value) : PageTableEntry(value) {}
        void Print() const {
            krnl::Printf_info("PT Entry");
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
        inline constexpr PageTable(Address address) : entries(reinterpret_cast<EntryType*>(address)) {}
        
        inline constexpr EntryType& operator[](size_t index) { return entries[index]; }
        inline constexpr const EntryType& operator[](size_t index) const { return entries[index]; }
    };

    class PML4 : public PageTable<PML4Entry> {
       public:
        template <typename Address>
        inline constexpr PML4(Address address) : PageTable(address) {}

        template <typename Address>
        inline PML4 CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };

    class PDPT : public PageTable<PDPTEntry> {
       public:
        template <typename Address>
        inline constexpr PDPT(Address address) : PageTable(address) {}

        template <typename Address>
        inline PDPT CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };

    class PD : public PageTable<PDEntry> {
       public:
        template <typename Address>
        inline constexpr PD(Address address) : PageTable(address) {}

        template <typename Address>
        inline PD CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };

    class PT : public PageTable<PTEntry> {
       public:
        template <typename Address>
        inline constexpr PT(Address address) : PageTable(address) {}

        template <typename Address>
        inline PT CopyTo(Address address) {
            return BaseCopyTo(address);
        }
    };


    inline PDPT PML4Entry::Fetch() {
        return PDPT(getVirtualAddress());
    }

    inline PD PDPTEntry::Fetch() {
        return PD(getVirtualAddress());
    }

    inline PT PDEntry::Fetch() {
        return PT(getVirtualAddress());
    }
}

namespace paging {
    extern page_table::PML4 KernelPML4;

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

    void Init();
}