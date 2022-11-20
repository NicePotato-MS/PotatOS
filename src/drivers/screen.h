#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>
#include "lowlevel.h"

# define VIDEO_ADDRESS (unsigned char*) 0xb8000
# define MAX_ROWS 25
# define MAX_COLS 80
// Attribute byte for our default colour scheme .
# define WHITE_ON_BLACK 0x0f
// Screen device I/O ports
# define REG_SCREEN_CTRL 0x3D4
# define REG_SCREEN_DATA 0x3D5

// Assign variables

// End of variables

/* Print character char at x y with attribute byte */
void print_char(char character, int x, int y, char attribute_byte) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

    //if attribute byte not specified, assume white on black
    if(!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

}

/* enable text mode cursor */
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(REG_SCREEN_CTRL, 0x0A);
	outb(REG_SCREEN_DATA, (inb(REG_SCREEN_DATA) & 0xC0) | cursor_start);
 
	outb(REG_SCREEN_CTRL, 0x0B);
	outb(REG_SCREEN_DATA, (inb(REG_SCREEN_DATA) & 0xE0) | cursor_end);
}

/* disable text mode cursor */
void disable_cursor() {
	outb(REG_SCREEN_CTRL, 0x0A);
	outb(REG_SCREEN_DATA, 0x20);
}

/*Get cursor offset */
uint16_t cursor_offset(void) {
    uint16_t pos = 0;
    outb(REG_SCREEN_CTRL, 0x0F);
    pos = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 0x0E);
    pos += inb(REG_SCREEN_DATA);
    return pos;
}

/* Get cursor x */
uint16_t cursor_x(void) {
    return cursor_offset() % MAX_COLS;
}

/* Get cursor y */
uint16_t cursor_y(void) {
    return cursor_offset() / MAX_ROWS;
}

/* Set cursor position to x y */
void set_cursor_pos(int x, int y) {
	uint16_t pos = y * MAX_COLS + x;
 
	outb(REG_SCREEN_CTRL, 0x0F);
	outb(REG_SCREEN_DATA, (uint8_t) (pos & 0xFF));
	outb(REG_SCREEN_CTRL, 0x0E);
	outb(REG_SCREEN_DATA, (uint8_t) ((pos >> 8) & 0xFF));
}

/* Scroll screen up by scroll amount */
void scroll_screen(uint16_t scroll_amount) {
    //Error checking to avoid memory leaks
    //Check if scroll amount is greater than MAX_ROWS negative or positive
    if(scroll_amount>MAX_ROWS) {
        scroll_amount = MAX_ROWS;
    }

    uint16_t byte_amount = MAX_ROWS-scroll_amount*MAX_COLS*2;

    nmemcpy(
    /*Destination*/ VIDEO_ADDRESS
    /*Source*/,     VIDEO_ADDRESS+scroll_amount*MAX_COLS*2
    /*Bytes*/,      byte_amount);

    //Loop through remaining space and clear it
    int i;
    unsigned char* vidPointer = VIDEO_ADDRESS;
    for (i = MAX_ROWS*MAX_COLS-(scroll_amount*MAX_COLS)*2; i<MAX_ROWS*MAX_COLS; i++)
    {
        vidPointer[i] = 0;
    }
    

}

/* Check if the screen needs to scroll. */
uint16_t check_scrolling ( int cursor_offset ) {
    // If the cursor is within the screen, return unmodified.
    if(cursor_offset<MAX_ROWS*MAX_COLS*2) {
        return cursor_offset;
    } 
    /* Shift screen up one */
    scroll_screen(1);

    // Move the offset back one row , such that it is now on the last
    // row , rather than off the edge of the screen .
    cursor_offset -= 2*MAX_COLS;
    // Return the updated cursor position .
    return cursor_offset;
}

/* Clear screen */
void clear_screen() {
    int i;
    unsigned char* vidPointer = VIDEO_ADDRESS;
    for (i = 0; i<MAX_ROWS*MAX_COLS; i++)
    {
        vidPointer[i] = 0;
    }
}