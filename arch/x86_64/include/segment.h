#pragma once

#define SYS_SD_PRESENT 1 << 7

#define SYS_SD_RING_0 0b00 << 5  // Kernel
#define SYS_SD_RING_1 0b01 << 5
#define SYS_SD_RING_2 0b10 << 5
#define SYS_SD_RING_3 0b11 << 5  // Userspace

#define SYS_SD_TSS 0 << 4  // Task State Segment
#define SYS_SD_CDS 1 << 4  // Code/Data Segment

#define SYS_SD_TYPE_16_AVAILABLE 0b0001
#define SYS_SD_TYPE_16_BUSY 0b0011
#define SYS_SD_TYPE_16_CALL 0b0100
#define SYS_SD_TYPE_16_INTERRUPT 0b0110
#define SYS_SD_TYPE_16_TRAP 0b0111
#define SYS_SD_TYPE_AVAILABLE 0b1001
#define SYS_SD_TYPE_BUSY 0b1011
#define SYS_SD_TYPE_CALL 0b1100
#define SYS_SD_TYPE_INTERRUPT 0b1110
#define SYS_SD_TYPE_TRAP 0b1111
#define SYS_SD_TYPE_LDT 0b0010
#define SYS_SD_TYPE_TASK 0b0101