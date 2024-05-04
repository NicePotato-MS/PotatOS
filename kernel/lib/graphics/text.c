#include <stddef.h>
#include <stdlib.h>
#include <text.h>

void extractFont(bitmap_font font) {
    // Load packed data to bytes in loaded
    for (size_t byte = 0; byte < font.packed_size; byte++) {
        for (uint8_t bit = 0; bit < 7; bit++) {
            font.loaded[byte + bit] = (font.data[byte] >> bit) & 1;
        }
    }
}