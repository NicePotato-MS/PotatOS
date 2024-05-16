#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#include <serial.h>

SerialPort COM1;

typedef struct {
    SerialPort *port;
    size_t written;
} serial_putchar_args;

// Internal serial putchar function
static int serial_putchar(const char *chars, void *args_void, size_t size) {
    serial_putchar_args* args = static_cast<serial_putchar_args *>(args_void);
    while (size--) {
        args->port->WriteByte(*chars++);
    }
    args->written += size;
    return 0;
}

/**
 * Writes formatted string to Serial port with existing va_list
 *
 * @param port Serial port to write to
 * @param str string to format
 * @param va va_list
 * @return amount of characters written
 */
int SerialPort::WritefVarg(const char *str, va_list va) {
    serial_putchar_args putchar_args = {this, 0};
    format(serial_putchar, &putchar_args, str, va);
    return putchar_args.written;
}

/**
 * Writes formatted string to Serial port
 *
 * @param port Serial port to write to
 * @param str string to format
 * @return amount of characters written
 */
int SerialPort::Writef(const char *str, ...) {
    va_list va;
    va_start(va, str);
    size_t written = WritefVarg(str, va);
    va_end(va);
    return written;
}

bool SerialPort::Init(uint16_t port, uint16_t divisor) {
    port_id = port;
    SetInterruptEnabled(SRL_INTERRUPT_OFF);
    SetDivisorUnsafe(divisor);
    SetLineControlRegister(
        SRL_DATA_BITS_8 | SRL_PARITY_NONE | SRL_STOP_BITS_1);
    SetFIFOControlRegister(
        SRL_FIFO_ON | SRL_FIFO_RECEIVE_CLEAR | SRL_FIFO_TRANSMIT_CLEAR |
        SRL_TRIGGER_LEVEL_14);
    // Test Serial port with Local Loopback mode
    SetModemControlRegister(
        SRL_MODEM_RTS | SRL_MODEM_OUT1 | SRL_MODEM_OUT2 | SRL_MODEM_LOOPBACK);
    WriteByte(0xAE);
    if (ReadByte() != 0xAE) {
        // Serial port faulty
        Writef("False Failure");
        return false;
    }
    // Serial port working
    SetModemControlRegister(SRL_MODEM_ON);
    initialized = true;
    return true;
}

