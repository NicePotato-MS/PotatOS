#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_set_cursor_position(uint16_t x, uint16_t y);
uint16_t terminal_get_cursor_position(void);
void terminal_enable_cursor(void);
void terminal_disable_cursor(void);
void terminal_increment_row(uint8_t inc_amount);
void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
