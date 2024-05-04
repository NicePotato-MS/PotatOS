#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <kernel.h>
#include <text.h>

#include <fonts/dina_7x16.h>

#include <limine.h>

// Set the base revision to 1, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

LIMINE_BASE_REVISION(1)

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, in C, they should
// NOT be made "static".

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void _kmain(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if(LIMINE_BASE_REVISION_SUPPORTED == false) {
        halt();
    }

    // Ensure we got a framebuffer.
    if(framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        halt();
    }

    // Fetch the first framebuffer.

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    uint32_t *fb_addr = framebuffer->address;
    uint64_t fb_width = framebuffer->width;
    uint64_t fb_height = framebuffer->height;

    extractFont(dina_7x16);

    size_t chr = 0;

    for (size_t y = 0; y < 16; y++) {
        for (size_t x = 0; x < 16; x++) {
            for (size_t cy = 0; cy < dina_7x16.size_y; cy++) {
                for (size_t cx = 0; cx < dina_7x16.size_x; cx++) {
                    //uint8_t color = dina_7x16.loaded[chr] * 255;
                    uint8_t color = chr;
                    fb_addr[(y * fb_width * dina_7x16.size_y) + (x * dina_7x16.size_x) + (cy * fb_width) + cx] = (color << 16 | color << 8 | color);
                }
            }
            chr = chr+2;
        }
    }

        // for (size_t y = 0; y < dina_7x16.size_y; y++) {
        //     for (size_t x = 0; x < dina_7x16.size_x; x++) {
        //         uint8_t color = dina_7x16.loaded[(y * dina_7x16.size_x + x) + (dina_7x16.glyph_size * 65)] * 255;
        //         fb_addr[y * fb_width + x] = (color << 16 | color << 8 | color);
        //     }
        // }

        // Note: we assume the framebuffer model is RGB with 32-bit pixels.
        // for(size_t y = 0; y < fb_height; y++) {
        //     for(size_t x = 0; x < fb_width; x++) {
        //         uint8_t r = (255 * x) / (fb_width - 1);
        //         uint8_t g = (255 * y) / (fb_height - 1);
        //         uint8_t b = 255 - r;

        //         fb_addr[y * fb_width + x] = (r<<16 | g<<8 | b);
        //     }
        // }

        // We're done, just hang...
        halt();
}