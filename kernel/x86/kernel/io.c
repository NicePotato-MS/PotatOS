#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>

#include <io.h>

srl_Port srl_COM1 = {
    PORT_COM1,
    false
};

bool srl_Init(srl_Port port, uint16_t divisor) {
    srl_SetInterruptEnabled(port, SRL_INTERRUPT_OFF);
    srl_SetDivisorUnsafe(port, divisor);
    srl_SetLineControlRegister(
        port, SRL_DATA_BITS_8 | SRL_PARITY_NONE | SRL_STOP_BITS_1);
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
    return true;
}