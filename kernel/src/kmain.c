#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <kernel.h>

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

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for(size_t y = 0; y < fb_height; y++) {
        for(size_t x = 0; x < fb_width; x++) {
            uint8_t r = (255 * x) / (fb_width - 1);
            uint8_t g = (255 * y) / (fb_height - 1);
            uint8_t b = 255 - r;

            fb_addr[y * fb_width + x] = (r<<16 | g<<8 | b);
        }
    }

    // We're done, just hang...
    halt();
}