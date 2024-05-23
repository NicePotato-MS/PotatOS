#include <stdint.h>

#include <pic.h>
#include <io.h>

void pic::Init() {
    MasterCommand(PIC_INITIALIZE | PIC_CW4_PRESENT);
    SlaveCommand(PIC_INITIALIZE | PIC_CW4_PRESENT);
    MasterDataOut(PIC_MASTER_OFFSET);
    SlaveDataOut(PIC_SLAVE_OFFSET);
    MasterDataOut(0b00000010);  // 0 - No slave 1 - Slave IR
    SlaveDataOut(2);            // Slave ID 2
    MasterDataOut(PIC_8086_8088_MODE);
    SlaveDataOut(PIC_8086_8088_MODE);

    MasterDataOut(PIC_MODIFY_MASK | 0b1111111);  // Disable All
    SlaveDataOut(PIC_MODIFY_MASK | 0b1111111);
}