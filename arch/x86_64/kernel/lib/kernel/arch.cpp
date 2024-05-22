#include <stdint.h>

#include <kernel.h>
#include <interrupts.h>
#include <gdt.h>
#include <arch.h>
#include <limine.h>
#include <tty.h>

#include <fonts/dina_7x16.h>
#include <fonts/dina_bold_7x16.h>
#include <palettes/xubunterm.h>

// Will cause page fault or undefined behaviour with other fonts
uint8_t term_buffer_text[182 * 45];
uint32_t term_buffer_bg[182 * 45];
uint32_t term_buffer_fg[182 * 45];

struct limine_framebuffer_request framebuffer_request = {
    LIMINE_FRAMEBUFFER_REQUEST, 0, NULL};

void arch::Setup() {
    gdt::Load();
    gdt::SetSegments(GDT_CS_KERNEL, GDT_DS_KERNEL); // 64-bit code, 64-bit data
    idt::Init();

    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
    } else {
        struct limine_framebuffer *lmn_fb =
            framebuffer_request.response->framebuffers[0];

        tty::Framebuffer fb;

        fb.vram = static_cast<uint32_t *>(lmn_fb->address);
        fb.width = lmn_fb->width;
        fb.height = lmn_fb->height;
        fb.pitch = lmn_fb->pitch;
        fb.bpp = lmn_fb->bpp;
        fb.font = dina_7x16;

        tty::tty1.Init(fb, dina_7x16, dina_bold_7x16, xubunterm_palette, term_buffer_text,
            term_buffer_bg, term_buffer_fg);
    }
}