#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <lowlevel.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static bool terminal_cursor_enabled = false; // Since we have no idea if the cursor is actually enabled or not, assume it isn't
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_set_cursor_position(uint16_t x, uint16_t y)
{
	uint16_t pos = y * VGA_WIDTH + x;
	terminal_column = x;
	terminal_row = y;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t terminal_get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

void terminal_enable_cursor(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D4, 0x0B);
	terminal_cursor_enabled = true;
}

void terminal_disable_cursor(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
	terminal_cursor_enabled = false;
}

void terminal_shift_screen(int8_t shift) { // Positive values moves up, negative moves down

}

void terminal_increment_row(uint8_t inc_amount) {
	if (++terminal_row == VGA_HEIGHT)
		terminal_row = 0;
	if (terminal_cursor_enabled == true)
		terminal_set_cursor_position(terminal_column, terminal_row);
}

void terminal_initialize(void) {
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	terminal_enable_cursor();
	terminal_set_cursor_position(0,0);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	if (c == '\n') { // Check if current character is a newline
		terminal_column = 0;
		terminal_increment_row(1);
	} else {
		terminal_buffer[index] = vga_entry(c, color);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			terminal_increment_row(1);
		}
	}
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (terminal_cursor_enabled == true)
		terminal_set_cursor_position(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}