#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <io.h>

#define PORT_COM1 0x3F8  // Serial Port 1
#define PORT_COM2 0x2F8  // Serial Port 2
#define PORT_COM3 0x3E8  // Serial Port 3
#define PORT_COM4 0x2E8  // Serial Port 4
#define PORT_COM5 0x5F8  // Serial Port 5
#define PORT_COM6 0x4F8  // Serial Port 6
#define PORT_COM7 0x5E8  // Serial Port 7
#define PORT_COM8 0x4E8  // Serial Port 8

#define SRL_REG_DATA 0              // Data Register offset
#define SRL_REG_INTERRUPT_ENABLE 1  // Interrupt Enable Register offset
#define SRL_REG_DIV_LOW 0           // Divisor Value Low offset
#define SRL_REG_DIV_HIGH 1          // Divisor Value High offset
#define SRL_REG_INTERRUPT_ID 2      // Interrupt Identification Register offset
#define SRL_REG_FIFO 2              // FIFO Control Register offset
#define SRL_REG_LINE_CONTROL 3      // Line Control Register offset
#define SRL_REG_MODEM_CONTROL 4     // Modem Control Register offset
#define SRL_REG_LINE_STATUS 5       // Line Status Register offset
#define SRL_REG_MODEM_STATUS 6      // Modem Status Register offset

#define SRL_INTERRUPT_OFF 0           // Disable all interrupts
#define SRL_INTERRUPT_DATA 0b1        // Received Data Available interrupt
#define SRL_INTERRUPT_EMPTY 0b1 << 1  // Transmitter Register Empty interrupt
#define SRL_INTERRUPT_LINE 0b1 << 2   // Receiver Line Status interrupt
#define SRL_INTERRUPT_MODEM 0b1 << 3  // Modem Status interrupt
#define SRL_INTERRUPT_ON 0b1111

#define SRL_BAUD_115200 1
#define SRL_BAUD_57600 2
#define SRL_BAUD_38400 3
#define SRL_BAUD_28800 4
#define SRL_BAUD_19200 6
#define SRL_BAUD_14400 8
#define SRL_BAUD_9600 12
#define SRL_BAUD_4800 24
#define SRL_BAUD_2400 46
#define SRL_BAUD_1200 96
#define SRL_BAUD_600 192
#define SRL_BAUD_300 384

#define SRL_DATA_BITS_5 0b00  // 5 Data Bits
#define SRL_DATA_BITS_6 0b01  // 6 Data Bits
#define SRL_DATA_BITS_7 0b10  // 7 Data Bits
#define SRL_DATA_BITS_8 0b11  // 8 Data Bits

#define SRL_STOP_BITS_1 0b0 << 2  // 1 Stop bit
#define SRL_STOP_BITS_2 0b1 << 2  // 2 or 1.5 Stop bits

#define SRL_PARITY_NONE 0b000 << 3
#define SRL_PARITY_ODD 0b001 << 3
#define SRL_PARITY_EVEN 0b011 << 3
#define SRL_PARITY_MARK 0b101 << 3
#define SRL_PARITY_SPACE 0b111 << 3

#define SRL_BREAK_DISABLE 0b0 << 6  // Disable Break
#define SRL_BREAK_ENABLE 0b1 << 6   // Enable Break

#define SRL_DLAB_OFF 0b0 << 7  // Disable Divisor Latch Access
#define SRL_DLAB_ON 0b1 << 7   // Enable Divisor Latch Access

#define SRL_MODEM_DTR 0b1       // Data Terminal Ready
#define SRL_MODEM_RTS 0b1 << 1  // Request To Send
#define SRL_MODEM_OUT1 0b1 << 2
#define SRL_MODEM_OUT2 0b1 << 3
#define SRL_MODEM_LOOPBACK 0b1 << 4  // Local loopback mode
#define SRL_MODEM_ON 0b1111  // Normal operation mode, DTR | TRS | OUT1 | OUT2

#define SRL_DATA_READY 0b1            // Line Status Data Ready
#define SRL_OVERRUN_ERROR 0b1 << 1    // Line Status Overrun Error
#define SRL_PARITY_ERROR 0b1 << 2     // Line Status Parity Error
#define SRL_FRAMING_ERROR 0b1 << 3    // Line Status Framing Error
#define SRL_BREAK_INDICATOR 0b1 << 4  // Line Status Break Indicator
#define SRL_TRANSMITTER_HOLDING_EMPTY 0b1 << 5  // Line Status Transmitter Holding Register Empty
#define SRL_TRANSMITTER_EMPTY 0b1 << 6  // Line Status Transmitter Empty
#define SRL_IMPENDING_ERROR 0b1 << 7    // Line Status Impending Error

#define SRL_DELTA_CLEAR_TO_SEND 0b1        // Modem Status Delta Clear to Send
#define SRL_DELTA_DATA_SET_READY 0b1 << 1  // Modem Status Delta Data Set Ready
#define SRL_TRAILING_EDGE_RING_INDICATOR 0b1 << 2  // Modem Trailing Edge of Ring Indicator
#define SRL_DELTA_DATA_CARRIER_DETECT 0b1 << 3                         // Modem Status Delta Data Carrier Detect
#define SRL_CLEAR_TO_SEND 0b1 << 4   // Modem Status Clear to Send
#define SRL_DATA_SET_READY 0b1 << 5  // Modem Status Data Set Ready
#define SRL_RING_INDICATOR 0b1 << 6  // Modem Status Ring Indicator
#define SRL_DATA_CARRIER_DETECT 0b1 << 7  // Modem Status Carrier Detect

#define SRL_FIFO_OFF 0b0                  // Disable FIFO buffers
#define SRL_FIFO_ON 0b1                   // Enable FIFO buffers
#define SRL_FIFO_RECEIVE_CLEAR 0b1 << 1   // Clear FIFO Receive buffer
#define SRL_FIFO_TRANSMIT_CLEAR 0b1 << 2  // Clear FIFO Transmit buffer

#define SRL_TRIGGER_LEVEL_1 0b00 << 6   // Interrupt after 1 byte
#define SRL_TRIGGER_LEVEL_4 0b01 << 6   // Interrupt after 4 bytes
#define SRL_TRIGGER_LEVEL_8 0b10 << 6   // Interrupt after 8 bytes
#define SRL_TRIGGER_LEVEL_14 0b11 << 6  // Interrupt after 14 bytes

#define SRL_STATE_MODEM_STATUS 0b00 << 1  // Modem Status Interrupt State
#define SRL_STATE_TRANSMITTER_BUFFER_EMPTY 0b01 << 1  // Transmitter Buffer Empty Interrupt State
#define SRL_STATE_RECEIVED_DATA_AVAILABLE 0b10 << 1  // Received Data Available Interrupt State
#define SRL_STATE_RECEIVER_LINE_STATUS 0b11 << 1  // Receiver Line Status Interrupt State

#define SRL_NO_FIFO 0b00 << 6                // No FIFO
#define SRL_FIFO_ENABLED_UNUSABLE 0b01 << 6  // FIFO Enabled but Unusable
#define SRL_FIFO_ENABLED 0b10 << 6           // FIFO Enabled

typedef struct {
    uint16_t id;
    bool initalized;
} srl_Port;

extern srl_Port srl_COM1;

// Set Interrupt Enabled Register of Serial port
static inline void srl_SetInterruptEnabled(srl_Port port, uint8_t state) {
    outb(port.id + SRL_REG_INTERRUPT_ENABLE, state);
}

// Set Line Control Register of Serial port to state
static inline void srl_SetLineControlRegister(srl_Port port, uint8_t state) {
    outb(port.id + SRL_REG_LINE_CONTROL, state);
}

// Set First In / First Out Control Register of Serial port to state
static inline void srl_SetFIFOControlRegister(srl_Port port, uint8_t state) {
    outb(port.id + SRL_REG_FIFO, state);
}

// Set Modem Control Register of Serial port to state
static inline void srl_SetModemControlRegister(srl_Port port, uint8_t state) {
    outb(port.id + SRL_REG_MODEM_CONTROL, state);
}

// Set Divisor Value for Baud rate without preserving Line Control Register
static inline void srl_SetDivisorUnsafe(srl_Port port, uint16_t divisor) {
    srl_SetLineControlRegister(port, SRL_DLAB_ON);
    outb(port.id + SRL_REG_DIV_LOW, divisor & 0xFF);
    outb(port.id + SRL_REG_DIV_HIGH, (divisor >> 8) & 0xFF);
}

// Set Divisor Value for Baud rate while preserving Line Control Register
static inline void srl_SetDivisorSafe(srl_Port port, uint16_t divisor) {
    uint8_t lcr = inb(port.id + SRL_REG_LINE_CONTROL);
    srl_SetLineControlRegister(port, SRL_DLAB_ON);
    outb(port.id + SRL_REG_DIV_LOW, divisor & 0xFF);
    outb(port.id + SRL_REG_DIV_HIGH, (divisor >> 8) & 0xFF);
    srl_SetLineControlRegister(port, lcr);
}

// Read a byte from a Serial port
static inline uint8_t srl_ReadByte(srl_Port port) { return inb(port.id); }

static inline void srl_TransmitYield(srl_Port port) {
    while (!(inb(port.id + SRL_REG_LINE_STATUS) & SRL_TRANSMITTER_HOLDING_EMPTY));
}

// Write a byte to a Serial port
static inline void srl_WriteByte(srl_Port port, uint8_t data) {
    srl_TransmitYield(port); // This might be a bad idea
    outb(port.id, data);
}

/**
 * Writes formatted string to Serial port
 *
 * @param port Serial port to write to
 * @param str string to format
 * @return amount of characters written
 */
int srl_Writef(srl_Port port, const char *str, ...);

// Initialize Serial port with standard config and set Divisor Latch to divisor
// for Baud rate
// Returns if serial was initialized and working
bool srl_Init(srl_Port port, uint16_t divisor);

#ifdef __cplusplus
}
#endif