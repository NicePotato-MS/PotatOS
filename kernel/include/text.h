#ifndef _LIB_TEXT_HEADER
#define _LIB_TEXT_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint16_t size_x;
    uint16_t size_y;
    uint32_t glyph_size;
    uint32_t packed_size; // Size of data array
    uint8_t *data;
    uint8_t *loaded;
} bitmap_font;

void extractFont(bitmap_font font);

#ifdef __cplusplus
}
#endif

#endif