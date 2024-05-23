#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <ctype.h>

#include <kernel.h>
#include <tty.h>
#include <ansi.h>

#include <palettes/xubunterm.h>

void tty::Framebuffer::DrawGlyphNoClip(uint32_t x, uint32_t y, uint8_t chr, uint32_t bg, uint32_t fg) {
    for (uint32_t cy = 0; cy < font.size_y; cy++) {
        uint32_t offset = y * width + pitch + x;
        for (uint32_t cx = 0; cx < font.size_x; cx++) {
            uint32_t color;
            if (font.data[font.size_y * (uint8_t)chr + cy] & (0b10000000 >> cx)) {
                color = fg;
            } else {
                color = bg;
            }
            vram[offset + cx] = color;
        }
        y++;
    }
}

void tty::Terminal::ScrollScreenUp(uint16_t amount) {
    uint32_t cx = 0;
    uint32_t cy = 0;
    uint16_t i = 0;
    for (; i < size_total - (amount * (size_x)); i++) {
        buffer_text[i] = buffer_text[i + amount * size_x];
        buffer_bg[i] = buffer_bg[i + amount * size_x];
        buffer_fg[i] = buffer_fg[i + amount * size_x];
        fb.DrawGlyphNoClip(
            cx, cy, buffer_text[i], buffer_bg[i], buffer_fg[i]);
        cx += font.size_x;
        if (cx >= font.size_x * (size_x)) {
            cx = 0;
            cy += font.size_y;
        }
    }

    for (; i < size_total; i++) { // I am aware this is unoptimized
        buffer_text[i] = 0;
        buffer_bg[i] = 0;
        buffer_fg[i] = 0;
        fb.DrawGlyphNoClip(cx, cy, ' ', 0, 0);
        cx += font.size_x;
        if (cx >= font.size_x * (size_x)) {
            cx = 0;
            cy += font.size_y;
        }
    }

    y -= amount;
}

void tty::Terminal::SetCursorX(uint16_t new_x) {
    if (new_x > size_x - 1) {
        x = size_x - 1;
    } else {
        x = new_x;
    }
}
void tty::Terminal::SetCursorY(uint16_t new_y) {
    if (new_y > size_y - 1) {
        y = size_y - 1;
    } else {
        y = new_y;
    }
}

void tty::Terminal::SetCursor(uint16_t new_x, uint16_t new_y) {
    SetCursorX(new_x);
    SetCursorY(new_y);
}


void tty::Terminal::MoveCursorX(int amount) {
    int new_x = x + amount;
    if (new_x > size_x - 1) {
        x = size_x - 1;
    } else if (new_x < 0) {
        x = 0;
    } else {
        x = new_x;
    }
}

void tty::Terminal::MoveCursorY(int amount) {
    int new_y = y + amount;
    if (new_y > size_y - 1) {
        y = size_y - 1;
    } else if (new_y < 0) {
        y = 0;
    } else {
        y = new_y;
    }
}

#warning Terminal MoveCursor%Scroll functions are missing negative movement

void tty::Terminal::MoveCursorXScroll(int amount) {
    if (amount < 0) {
    } else {
        while (amount > size_x) {
            ScrollScreenUp(1);
            amount -= size_x;
        }
        x += amount;
        if (x > size_x) {
            ScrollScreenUp(1);
            x -= size_x;
        }
    }
}

void tty::Terminal::MoveCursorYScroll(int amount) {
    if (amount < 0) {

    } else {
        while (amount--) {
            ScrollScreenUp(1);
        }
    }
}

void tty::Terminal::Putchar(const char chr) {
    if (ANSI_SkipOrParseCheck(chr)) {
        size_t buffer_index;
        switch (chr) {
            case '\b':
                if (x > 0) {
                    x--;
                } else {
                    if (y > 0) {
                        y--;
                        x = size_x - 1;
                    }
                }
                fb.DrawGlyphNoClip(x * font.size_x, y * font.size_y, ' ', 0, 0);
                buffer_index = y * size_x + x;
                buffer_text[buffer_index] = ' ';
                buffer_bg[buffer_index] = palette[0];
                buffer_fg[buffer_index] = palette[15];
                break;
            case '\n':
                if (++y > size_y - 1) { ScrollScreenUp(1); }
                // fallthrough
            case '\r': x = 0; break;
            default:
                fb.DrawGlyphNoClip(
                    x * font.size_x, y * font.size_y, chr, bg, fg);
                buffer_index = y * size_x + x;
                buffer_text[buffer_index] = (uint8_t)(chr);
                buffer_bg[buffer_index] = bg;
                buffer_fg[buffer_index] = fg;

                if (++x > size_x - 1) {
                    if (++y > size_y - 1) { ScrollScreenUp(1); }
                    x = 0;
                }
        }
    }
}

#define ANSI_DONE() ansi_mode = ANSI_PARSE_MODE_ESC; return false;

bool tty::Terminal::ANSI_SkipOrParseCheck(uint8_t chr) {
    uint32_t tmp;
    uint32_t arg_1;

    switch (ansi_mode) {
        case ANSI_PARSE_MODE_ESC:
            if (chr == '\e') {
                for (size_t i = 0; i < ARRAY_SIZE(ansi_arg_buffer); i++) {
                    ansi_arg_buffer[i] = 0;
                }
                ansi_arg_p = 0;
                ansi_mode = ANSI_PARSE_MODE_INTRODUCER;
                return false;
            }
            return true;
        case ANSI_PARSE_MODE_INTRODUCER:
            if (chr == '[') { ansi_mode = ANSI_PARSE_MODE_CSI_COMMAND; }
            return false;
        case ANSI_PARSE_MODE_CSI_COMMAND:
            if (isdigit(chr)) {
                ansi_arg_buffer[ansi_arg_p] *= 10;
                ansi_arg_buffer[ansi_arg_p] += chr - '0';
                return false;
            }
            switch (chr) {
                case ';':
                    if (ansi_arg_p < ARRAY_SIZE(ansi_arg_buffer) - 1) { ansi_arg_p++; }
                    
                    return false;
                case ' ': // Not standard, but I don't care right now
                    return false;
                case 'F': // Move Cursor Up to beginning
                    x = 0;
                    // fallthrough
                case 'A': // Move Cursor Up
                    if (!ansi_arg_buffer[0]) {
                        MoveCursorY(-1);
                    } else {
                        MoveCursorY(-ansi_arg_buffer[0]);
                    }
                    ANSI_DONE();
                case 'E': // Move Cursor Down to beginning
                    x = 0;
                    // fallthrough
                case 'B': // Move Cursor Down
                    if (!ansi_arg_buffer[0]) {
                        MoveCursorY(1);
                    } else {
                        MoveCursorY(ansi_arg_buffer[0]);
                    }
                    ANSI_DONE();
                case 'C': // Move Cursor Right
                    if (!ansi_arg_buffer[0]) {
                        MoveCursorX(1);
                    } else {
                        MoveCursorX(ansi_arg_buffer[0]);
                    }
                    ANSI_DONE();
                case 'D': // Move Cursor Left
                    if (!ansi_arg_buffer[0]) {
                        MoveCursorX(-1);
                    } else {
                        MoveCursorX(-ansi_arg_buffer[0]);
                    }
                    ANSI_DONE();
                case 'H':  // Move Cursor to Position
                    SetCursor(ansi_arg_buffer[0], ansi_arg_buffer[1]);
                    ANSI_DONE();
                case 'm':
                    arg_1 = ansi_arg_buffer[0];
                    if (30 <= arg_1 && arg_1 <= 37) {  // FG Color
                        fg = palette[arg_1 - 30];
                        ANSI_DONE();
                    }
                    if (40 <= arg_1 && arg_1 <= 47) { // BG Color
                        bg = palette[arg_1 - 40];
                        ANSI_DONE();
                    }
                    if (90 <= arg_1 && arg_1 <= 97) { // FG Color Bright
                        fg = palette[arg_1 - 82];
                        ANSI_DONE();
                    }
                    if (100 <= arg_1 && arg_1 <= 107) { // BG Color Bright
                        bg = palette[arg_1 - 92];
                        ANSI_DONE();
                    }
                    switch (arg_1) {
                        case 0: // Reset Attributes
                            bg = palette[0];
                            fg = palette[15];
                            bold = false;
                            fb.font = font;
                            ANSI_DONE();
                        case 1: // Bold
                            bold = true;
                            fb.font = font_bold;
                            ANSI_DONE();
                        case 7:  // Reverse Colors:
                            tmp = fg;
                            fg = bg;
                            bg = tmp;
                            ANSI_DONE();
                        case 22:  // Normal Intensity
                            bold = false;
                            fb.font = font;
                            ANSI_DONE();
                        case 38:
                            if (ansi_arg_buffer[1] == 5) {
                                if (ansi_arg_buffer[2] > 255) {
                                    fg = palette[7];
                                } else {
                                    fg = palette[ansi_arg_buffer[2]];
                                }
                                ANSI_DONE()
                            } else if (ansi_arg_buffer[1] == 2) {
                                fg = (((uint8_t)ansi_arg_buffer[2]) << 16 |
                                      ((uint8_t)ansi_arg_buffer[3]) << 8 |
                                      ((uint8_t)ansi_arg_buffer[4]));
                                ANSI_DONE();
                            }
                            ANSI_DONE();
                        case 39:  // Reset FG Color
                            fg = palette[7];
                            ANSI_DONE();
                        case 48:
                            if (ansi_arg_buffer[1] == 5) {
                                if (ansi_arg_buffer[2] > 255) {
                                    bg = palette[0];
                                } else {
                                    bg = palette[ansi_arg_buffer[2]];
                                }
                                ANSI_DONE()
                            } else if (ansi_arg_buffer[1] == 2) {
                                bg = (((uint8_t)ansi_arg_buffer[2]) << 16 |
                                      ((uint8_t)ansi_arg_buffer[3]) << 8 |
                                      ((uint8_t)ansi_arg_buffer[4]));
                                ANSI_DONE();
                            }
                            ANSI_DONE();
                        case 49:  // Reset BG Color
                            bg = palette[0];
                            ANSI_DONE();
                        default:
                            ANSI_DONE();
                    }
                default:
                    krnl::Printf_warn("Unimplemented ANSI control %#X", chr);
                    ANSI_DONE();
            }
        default:
            return false;
    }
}

void tty::Terminal::Putchar(const char *chr, size_t size) {
    while (size--) { Putchar(*chr++); }
}

static int putchar(const char *chr, void * args, size_t size) {
    while (size--) { static_cast<tty::Terminal *>(args)->Putchar(*chr++); }
    return 0;
}

void tty::Terminal::Printf(const char *str, ...) {
    va_list va;
    va_start(va, str);
    format(putchar, this, str, va);
    va_end(va);
}

void tty::Terminal::ClearScreen() {
    memset(fb.vram, 0, (fb.width * fb.height) + (fb.pitch * fb.height / sizeof(fb.vram[0])));
}

bool tty::Terminal::Init(Framebuffer in_fb, bitmap_font in_font, bitmap_font in_font_bold, uint32_t *in_palette, uint8_t *in_buffer_text,
   uint32_t *in_buffer_bg, uint32_t *in_buffer_fg) {
    if (in_fb.vram == NULL) {
        initialized = false;
        return false;
    } else {
        fb = in_fb;
        x = 0;
        y = 0;
        font = in_font;
        font_bold = in_font_bold;
        fb.font = font;
        palette = in_palette;
        size_x = fb.width / font.size_x;
        size_y = fb.height / font.size_y;
        size_total = Terminal::size_x * Terminal::size_y;
        bg = palette[0];
        fg = palette[7];
        buffer_text = in_buffer_text;
        buffer_bg = in_buffer_bg;
        buffer_fg = in_buffer_fg;
        ansi_mode = ANSI_PARSE_MODE_ESC;
        bold = false;
        initialized = true;
        return true;
    }
}

tty::Terminal tty::tty1;