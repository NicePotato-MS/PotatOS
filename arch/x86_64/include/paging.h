#pragma once

#include <types.h>

#include <memory.h>
#include "kernel.h"

namespace page {
    enum {
        SIZE = 4096,
    };
}
namespace page_table {
    enum {
        ENTRY_COUNT = 512
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

    class PageTableEntry {
       private:
        size_t entry;

       public:
        inline PageTableEntry(size_t value) : entry(value) {}

        inline bool getPresent() const { return (entry >> shift::PRESENT) & 1; }
        inline bool getReadWrite() const { return (entry >> shift::READ_WRITE) & 1; }
        inline bool getPrivilege() const { return (entry >> shift::PRIVELEGE) & 1; }
        inline bool getWriteMode() const { return (entry >> shift::WRITE_THROUGH) & 1; }
        inline bool getCacheMode() const { return (entry >> shift::CACHE_DISABLE) & 1; }
        inline bool getAccessed() const { return (entry >> shift::ACCESSED) & 1; }

        inline size_t getRaw() const { return entry; }

        void PrintBase() {
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
        void Print() {
            krnl::Printf_info("PML4 Entry");
            PrintBase();
        }
    };

    class PDPTEntry : public PageTableEntry {
    };

    class PDEntry : public PageTableEntry {
    };

    class PTEntry : public PageTableEntry {
    };


    template <typename EntryType>
    class PageTable {
       protected:
        EntryType* entries;

       public:
        template <typename T>
        inline PageTable(T address) : entries(reinterpret_cast<EntryType*>(address)) {}

        inline EntryType& operator[](size_t index) { return entries[index]; }
        inline const EntryType& operator[](size_t index) const { return entries[index]; }

    };

    class PML4 : public PageTable<PML4Entry> {
       public:
        using PageTable<PML4Entry>::PageTable;
    };

    class PDPT : public PageTable<PDPTEntry> {
       public:
        using PageTable<PDPTEntry>::PageTable;
    };

    class PD : public PageTable<PDEntry> {
       public:
        using PageTable<PDEntry>::PageTable;
    };

    class PT : public PageTable<PTEntry> {
       public:
        using PageTable<PTEntry>::PageTable;
    };
}

namespace paging {
    inline size_t getCR3() {
        size_t cr3_value;
        __asm__ volatile (
            "mov %%cr3, %0"
            : "=r" (cr3_value)
            :
            : "memory"
        );
        return cr3_value;
    }

    inline void SetCR3(size_t address) {
        __asm__ volatile(
            "mov %0, %%cr3"
            :
            : "r"(address)
            : "memory"
        );
    }

    void Init();
}