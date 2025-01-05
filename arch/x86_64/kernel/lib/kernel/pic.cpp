#include <stdint.h>

#include <pic.h>
#include <io.h>

void pic::MaskOn(uint8_t interrupt) {
    uint16_t port;
    uint8_t mask;

    if (interrupt < 8) {
        port = PIC_MASTER_DATA;
    } else {
        port = PIC_SLAVE_DATA;
        interrupt -= 8;
    }
    mask = io::inb(port) | (1 << interrupt);
    io::outb(port, mask);
}

void pic::MaskOff(uint8_t interrupt) {
    uint16_t port;
    uint8_t mask;

    if (interrupt < 8) {
        port = PIC_MASTER_DATA;
    } else {
        port = PIC_SLAVE_DATA;
        interrupt -= 8;
    }
    mask = io::inb(port) & ~(1 << interrupt);
    io::outb(port, mask);
}

void pic::Init() {
    MasterCommandYield(PIC_INITIALIZE | PIC_CW4_PRESENT);
    SlaveCommandYield(PIC_INITIALIZE | PIC_CW4_PRESENT);
    MasterDataOutYield(PIC_MASTER_OFFSET);
    SlaveDataOutYield(PIC_SLAVE_OFFSET);
    MasterDataOutYield(0b00000100);  // 0 - No slave, 1 - Slave IR
    SlaveDataOutYield(2);            // Slave ID 2
    MasterDataOutYield(PIC_8086_8088_MODE);
    SlaveDataOutYield(PIC_8086_8088_MODE);

    MasterDataOut(PIC_MODIFY_MASK | 0b1111111);  // Disable All
    SlaveDataOut(PIC_MODIFY_MASK | 0b1111111);
}