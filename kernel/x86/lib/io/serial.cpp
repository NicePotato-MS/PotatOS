#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#include <serial.hpp>

srl_Port srl_COM1 = {PORT_COM1, false};

typedef struct {
    srl_Port *port;
    size_t written;
} serial_putchar_args;

// Internal serial putchar function
static int serial_putchar(const char *chars, void *args_void, size_t size) {
    serial_putchar_args* args = static_cast<serial_putchar_args *>(args_void);
    for (size_t chr = 0; chr < size; chr++) {
        srl_WriteByte(args->port, chars[chr]);
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
int srl_WritefVarg(srl_Port *port, const char *str, va_list va) {
    serial_putchar_args putchar_args = {port, 0};
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
int srl_Writef(srl_Port *port, const char *str, ...) {
    va_list va;
    va_start(va, str);
    size_t written = srl_WritefVarg(port, str, va);
    va_end(va);
    return written;
}

bool srl_Init(srl_Port *port, uint16_t divisor) {
    srl_SetInterruptEnabled(port, SRL_INTERRUPT_OFF);
    srl_SetDivisorUnsafe(port, divisor);
    srl_SetLineControlRegister(port,
        SRL_DATA_BITS_8 | SRL_PARITY_NONE | SRL_STOP_BITS_1);
    srl_SetFIFOControlRegister(
        port, SRL_FIFO_ON | SRL_FIFO_RECEIVE_CLEAR | SRL_FIFO_TRANSMIT_CLEAR |
        SRL_TRIGGER_LEVEL_14);
    // Test Serial port with Local Loopback mode
    srl_SetModemControlRegister(port,
        SRL_MODEM_RTS | SRL_MODEM_OUT1 | SRL_MODEM_OUT2 | SRL_MODEM_LOOPBACK);
    srl_WriteByte(port, 0xAE);
    if (srl_ReadByte(port) != 0xAE) {
        // Serial port faulty
        return false;
    }
    // Serial port working
    srl_SetModemControlRegister(port, SRL_MODEM_ON);
    port->initalized = true;
    return true;
}

