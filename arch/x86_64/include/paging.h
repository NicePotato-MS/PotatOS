#pragma once

#include <types.h>

#include <memory.h>

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

        inline bool GetPresent() const { return (entry >> shift::PRESENT) & 1; }
        inline bool GetReadWrite() const { return (entry >> shift::READ_WRITE) & 1; }
        inline bool GetPrivelege() const { return (entry >> shift::PRIVELEGE) & 1; }
        inline bool GetWriteThrough() const { return (entry >> shift::WRITE_THROUGH) & 1; }
        inline bool GetCacheDisable() const { return (entry >> shift::CACHE_DISABLE) & 1; }
        inline bool GetAccessed() const { return (entry >> shift::ACCESSED) & 1; }
    };

    template <typename Derived>
    class PageTable {
       protected:
        PageTableEntry entries[ENTRY_COUNT];

       public:
        inline PageTableEntry& operator[](size_t index) { return entries[index]; }
        inline const PageTableEntry& operator[](size_t index) const {return entries[index]; }
    };

    class PML4 : public PageTable<PML4> {};

    class PDPT : public PageTable<PDPT> {};

    class PD : public PageTable<PD> {};

    class PT : public PageTable<PT> {};
}

namespace paging {
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

    inline void SetCR3(size_t address) {
        __asm__ volatile(
            "mov %0, %%cr3"
            :
            : "r"(address)
            : "memory"
        );
    }
}