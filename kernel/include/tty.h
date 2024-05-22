#pragma once

#include <stdint.h>
#include <stddef.h>

namespace tty {
    typedef struct {
        uint16_t size_x;
        uint16_t size_y;
        uint32_t size_glyph;
        uint8_t *data;
    } bitmap_font;

    class Framebuffer {
       public:
        uint32_t *vram;
        uint32_t width;
        uint32_t height;
        uint32_t pitch;
        uint16_t bpp;
        bitmap_font font;

        void DrawGlyphNoClip(uint32_t x, uint32_t y, uint8_t chr, uint32_t bg, uint32_t fg);
        void DrawGlyph(uint32_t x, uint32_t y, uint8_t chr, uint32_t bg, uint32_t fg);
    };

    class Terminal {
       private:
        uint16_t ansi_mode;
        uint8_t ansi_arg_p;
        uint32_t ansi_arg_buffer[16];

        bool bold;
       public:
        Framebuffer fb;
        bitmap_font font;
        bitmap_font font_bold;
        uint32_t *palette;
        uint16_t x;
        uint16_t y;
        uint16_t size_x;
        uint16_t size_y;
        uint16_t size_total;
        uint32_t bg;
        uint32_t fg;
        bool initialized;

        uint8_t *buffer_text;
        uint32_t *buffer_bg;
        uint32_t *buffer_fg;

        void Putchar(const char *chr);
        void Putchar(const char *chr, size_t size);

        void Printf(const char *str, ...);

        void ScrollScreenUp(uint16_t amount);

        void SetCursorX(uint16_t new_x);
        void SetCursorY(uint16_t new_y);
        void SetCursor(uint16_t new_x, uint16_t new_y);
        
        void MoveCursorX(int amount);
        void MoveCursorY(int amount);
        void MoveCursorXScroll(int amount);
        void MoveCursorYScroll(int amount);

        void ClearScreen();

        void Init(Framebuffer in_fb, bitmap_font in_font, bitmap_font in_font_bold,
            uint32_t *in_palette, uint8_t *in_buffer_text, uint32_t *in_buffer_bg,
            uint32_t *in_buffer_fg);

        bool ANSI_SkipOrParseCheck(uint8_t chr);
    };
    
    extern Terminal tty1;
}
