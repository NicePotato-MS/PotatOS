#pragma once

#include <stddef.h>
#include <stdint.h>

#define PORT_COM1 0x3F8 // Serial Port 1
#define PORT_COM2 0x2F8 // Serial Port 2
#define PORT_COM3 0x3E8 // Serial Port 3
#define PORT_COM4 0x2E8 // Serial Port 4
#define PORT_COM5 0x5F8 // Serial Port 5
#define PORT_COM6 0x4F8 // Serial Port 6
#define PORT_COM7 0x5E8 // Serial Port 7
#define PORT_COM8 0x4E8 // Serial Port 8

#define SERIAL_REG_DATA 0             // Data Register offset
#define SERIAL_REG_INTERRUPT_ENABLE 1// Interrupt Enable Register offset
#define SERIAL_REG_BAUD_LOW 0         // Divisor Value Low offset
#define SERIAL_REG_BAUD_HIGH 1        // Divisor Value High offset
#define SERIAL_REG_INTERRUPT_ID 2  // Interrupt Identification Register offset
#define SERIAL_REG_FIFO 2          // FIFO Control Register offset
#define SERIAL_REG_LINE_CONTROL 3  // Line Control Register offset
#define SERIAL_REG_MODEM_CONTROL 4 // Modem Control Register offset
#define SERIAL_REG_LINE_STATUS 5   // Line Status Register offset
#define SERIAL_REG_MODEM_STATUS 6  // Modem Status Register offset

#define SERIAL_BAUD_115200_LOW 1
#define SERIAL_BAUD_115200_HIGH 0
#define SERIAL_BAUD_57600_LOW 2
#define SERIAL_BAUD_57600_HIGH 0
#define SERIAL_BAUD_38400_LOW 3
#define SERIAL_BAUD_38400_HIGH 0
#define SERIAL_BAUD_28800_LOW 4
#define SERIAL_BAUD_28800_HIGH 0
#define SERIAL_BAUD_19200_LOW 6
#define SERIAL_BAUD_19200_HIGH 0
#define SERIAL_BAUD_14400_LOW 8
#define SERIAL_BAUD_14400_HIGH 0
#define SERIAL_BAUD_9600_LOW 12
#define SERIAL_BAUD_9600_HIGH 0
#define SERIAL_BAUD_4800_LOW 24
#define SERIAL_BAUD_4800_HIGH 0
#define SERIAL_BAUD_2400_LOW 46
#define SERIAL_BAUD_2400_HIGH 0
#define SERIAL_BAUD_1200_LOW 96
#define SERIAL_BAUD_1200_HIGH 0
#define SERIAL_BAUD_600_LOW 192
#define SERIAL_BAUD_600_HIGH 0
#define SERIAL_BAUD_300_LOW 128
#define SERIAL_BAUD_300_HIGH 1

#define SERIAL_DATA_BITS_5 0b00 // 5 Data Bits
#define SERIAL_DATA_BITS_6 0b01 // 6 Data Bits
#define SERIAL_DATA_BITS_7 0b10 // 7 Data Bits
#define SERIAL_DATA_BITS_8 0b11 // 8 Data Bits

#define SERIAL_STOP_BITS_1 0b0 << 2 // 1 Stop bit
#define SERIAL_STOP_BITS_2 0b1 << 2 // 2 or 1.5 Stop bits

#define SERIAL_PARITY_NONE 0b000 << 5
#define SERIAL_PARITY_ODD 0b001 << 5
#define SERIAL_PARITY_EVEN 0b011 << 5
#define SERIAL_PARITY_MARK 0b101 << 5
#define SERIAL_PARITY_SPACE 0b111 << 5

#define SERIAL_BREAK_DISABLE 0b0 << 6 // Disable Break
#define SERIAL_BREAK_ENABLE 0b1 << 6  // Enable Break

#define SERIAL_DLAB_OFF 0b0 << 7 // Disable Divisor Latch Access
#define SERIAL_DLAB_ON 0b1 << 7  // Enable Divisor Latch Access

#define SERIAL_MODEN_DTR 0b1      // Data Terminal Ready
#define SERIAL_MODEM_RTS 0b1 << 1 // Request To Send
#define SERIAL_MODEM_OUT1 0b1 << 2
#define SERIAL_MODEM_OUT2 0b1 << 3
#define SERIAL_MODEM_LOOPBACK 0b1 << 4 // Local loopback mode
#define SERIAL_MODEN_ON 0b1111 // Normal operation mode, DTR | TRS | OUT1 | OUT2

#define SERIAL_FIFO_OFF 0b0                 // Disable FIFO buffers
#define SERIAL_FIFO_ON 0b1                  // Enable FIFO buffers
#define SERIAL_FIFO_RECEIVE_CLEAR 0b1 << 1  // Clear FIFO Receive buffer
#define SERIAL_FIFO_TRANSMIT_CLEAR 0b1 << 2 // Clear FIFO Transmit buffer

#define SERIAL_TRIGGER_LEVEL_1 0b00 << 7  // Interrupt after 1 byte
#define SERIAL_TRIGGER_LEVEL_4 0b01 << 7  // Interrupt after 4 bytes
#define SERIAL_TRIGGER_LEVEL_8 0b10 << 7  // Interrupt after 8 bytes
#define SERIAL_TRIGGER_LEVEL_14 0b11 << 7 // Interrupt after 14 bytes

static inline void outb(uint16_t port, uint8_t data) {
  __asm__ volatile("outb %b0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t data;
  __asm__ volatile("inb %w1, %b0" : "=a"(data) : "Nd"(port));
  return data;
}

static inline void outw(uint16_t port, uint16_t data) {
  __asm__ volatile("outw %w0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
  uint16_t data;
  __asm__ volatile("inw %w1, %w0" : "=a"(data) : "Nd"(port));
  return data;
}

static inline void outl(uint16_t port, uint32_t data) {
  __asm__ volatile("outl %0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
  uint32_t data;
  __asm__ volatile("inl %w1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

static inline void ioWait() { outb(0x80, 0); }