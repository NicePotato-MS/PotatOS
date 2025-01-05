#include <stdint.h>

#include <ps2.h>
#include <kernel.h>
#include <pic.h>
#include <io.h>
#include <interrupts.h>
#include <segment.h>

__attribute__((interrupt)) static void ps2_1_handler(void *) {
    char outbuf[2];
    outbuf[0] = io::inb(0x60);
    outbuf[1] = '\0';
    krnl::Printf("%s", outbuf);

    pic::EndOfInterruptMaster();
}

// __attribute__((interrupt)) static void ps2_2_handler(void *) {
//     char outbuf[2];
//     outbuf[0] = io::inb(0x60), 0;
//     krnl::Printf("%s", outbuf);

//     pic::EndOfInterruptMaster();
// }

void ps2::Init() {
    NEW_INTERRUPT(INTERRUPT_PS2_1, ps2_1_handler);
    pic::MaskOff(INTERRUPT_MASK_PS2_1);

    // NEW_INTERRUPT(INTERRUPT_PS2_2, ps2_2_handler);
    // pic::MaskOff(INTERRUPT_MASK_PS2_2);

    krnl::Printf_ok("PS/2 Driver Installed");
}