#pragma once

#include <stdint.h>

#include <io.h>

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA 0xA1

#define PIC_CW4_PRESENT 1
#define PIC_CW4_ABSENT 0

#define PIC_MASTER_ONLY 1 << 1
#define PIC_MASTER_SLAVE 0 << 1

#define PIC_INTERVAL_4 1 << 2
#define PIC_INTERVAL_8 0 << 2

#define PIC_LEVEL_MODE 1 << 3
#define PIC_EDGE_MODE 0 << 3 // (o.o )

#define PIC_INITIALIZE 1 << 4


#define PIC_MASTER_OFFSET 0x20
#define PIC_SLAVE_OFFSET 0x28


#define PIC_8086_8088_MODE 1
#define PIC_MCS_80_85_MODE 0

#define PIC_EOI_AUTO 1 << 1
#define PIC_EOI_MANUAL 0 << 1

#define PIC_NO_BUFFER 0b00 << 2
#define PIC_BUFFER_SLAVE 0b10 << 2
#define PIC_BUFFER_MASTER 0b11 << 2

#define PIC_SPECIAL_YES 1 << 4
#define PIC_SPECIAL_NO 0 << 4


#define PIC_MODIFY_MASK 1 << 7
#define PIC_MODIFY_INTERRUPT 0 << 7

namespace pic {
    inline void MasterCommand(uint8_t command) {
        io::outb(PIC_MASTER_COMMAND, command);
        io::io_Wait();
    }
    inline void MasterDataOut(uint8_t data) {
        io::outb(PIC_MASTER_DATA, data);
        io::io_Wait();
    }
    inline void SlaveCommand(uint8_t command) {
        io::outb(PIC_SLAVE_COMMAND, command);
        io::io_Wait();
    }
    inline void SlaveDataOut(uint8_t data) {
        io::outb(PIC_SLAVE_DATA, data);
        io::io_Wait();
    }
    void Init();
}