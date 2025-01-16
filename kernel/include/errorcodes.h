#pragma once

#include <stdint.h>

namespace errorcode {
    extern const char **categories[];
    extern const char *categoryNames[];
    extern const uint16_t categorySizes[];
    extern const uint16_t catergoryCount;

    enum {
        MISC = 0x00000000,
        MEMORY = 0x00010000,
        INTERRUPT = 0x00020000,
        MATH = 0x00030000,
        EXECUTION = 0x00040000,
    };

    namespace misc {
        enum {
            SUCCESS = MISC | 0x0000,
            BOOTLOADER_UNSUPPORTED = MISC | 0x0001,
            UNKNOWN = MISC | 0x0002
        };
    }

    namespace memory {
        enum {
            PAGE_FAULT = MEMORY | 0x0000,
            SEGMENTATION_FAULT = MEMORY | 0x0001,
            NO_MEMMAP = MEMORY | 0x0002,
            OUT_OF_PHYSICAL_MEMORY = MEMORY | 0x0003,
            OUT_OF_VIRTUAL_MEMORY = MEMORY | 0x0004
        };
    }

    namespace interrupt {
        enum {
            UNHANDLED = INTERRUPT | 0x0000,
            GENERAL_PROTECTION_FAULT = INTERRUPT | 0x0001
        };
    }

    namespace math {
        enum {
            DIVISION_BY_ZERO = MATH | 0x0000
        };
    }

    namespace execution {
        enum {
            INVALID_OPCODE = EXECUTION | 0x0000
        };
    }
}