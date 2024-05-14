#pragma once

#include <stdint.h>

typedef struct {
    uint16_t size_x;
    uint16_t size_y;
    uint32_t size_glyph;
    uint8_t *data;
} bitmap_font;