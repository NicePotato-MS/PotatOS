#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include "src/drivers/lowlevel.h"

# define VIDEO_ADDRESS 0xb8000
# define MAX_ROWS 25
# define MAX_COLS 80
// Attribute byte for our default colour scheme .
# define WHITE_ON_BLACK 0x0f
// Screen device I/O ports
# define REG_SCREEN_CTRL 0x3D4
# define REG_SCREEN_DATA 0x3D5

// Assign variables

// End of variables

void print_char(char character, int col, int row, char attribute_byte) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    //if attribute byte not specified, assume white on black
    if(!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(REG_SCREEN_CTRL, 0x0A);
	outb(REG_SCREEN_DATA, (inb(REG_SCREEN_DATA) & 0xC0) | cursor_start);
 
	outb(REG_SCREEN_CTRL, 0x0B);
	outb(REG_SCREEN_DATA, (inb(REG_SCREEN_DATA) & 0xE0) | cursor_end);
}

void disable_cursor() {
	outb(REG_SCREEN_CTRL, 0x0A);
	outb(REG_SCREEN_DATA, 0x20);
}

uint16_t cursor_offset(void) {
    uint16_t pos = 0;
    outb(REG_SCREEN_CTRL, 0x0F);
    pos = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 0x0E);
    pos += inb(REG_SCREEN_DATA);
    return pos;
}

uint16_t cursor_x(void) {
    return cursor_offset() % MAX_COLS;
}

uint16_t cursor_y(void) {
    return cursor_offset() / MAX_ROWS;
}

void set_cursor_pos(int x, int y) {
	uint16_t pos = y * MAX_COLS + x;
 
	outb(REG_SCREEN_CTRL, 0x0F);
	outb(REG_SCREEN_DATA, (uint8_t) (pos & 0xFF));
	outb(REG_SCREEN_CTRL, 0x0E);
	outb(REG_SCREEN_DATA, (uint8_t) ((pos >> 8) & 0xFF));
}

/* Advance the text cursor , scrolling the video buffer if necessary . */
int handle_scrolling ( int cursor_offset ) {
    // If the cursor is within the screen , return it unmodified .
    if(cursor_offset<MAX_ROWS*MAX_COLS*2) {
        return cursor_offset;
    }
    /* Shuffle the rows back one . */
    int i;
    for (i=1; i<MAX_ROWS; i++) {
        memcpy(VIDEO_ADDRESS, VIDEO_ADDRESS+MAX_COLS, MAX_ROWS*MAX_COLS-1);
    }
    /* Blank the last line by setting all bytes to 0 */
    char * last_line = get_screen_offset(0, MAX_ROWS-1)+VIDEO_ADDRESS;
    for (i = 0; i<MAX_COLS*2; i++) {
        last_line[i] = 0;
    }
    // Move the offset back one row , such that it is now on the last
    // row , rather than off the edge of the screen .
    cursor_offset -= 2*MAX_COLS;
    // Return the updated cursor position .
    return cursor_offset;
}

void clear_screen() {
    int row = 0;
    int col = 0;

    /*Loop through video ram and write spaces*/
    for (row=0; row<MAX_ROWS; row++) {
        for (col=0; col<MAX_COLS; col++) {

        }
    }
}