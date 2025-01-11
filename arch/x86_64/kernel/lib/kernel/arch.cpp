#include <stdint.h>

#include <kernel.h>
#include <memory.h> 
#include <interrupts.h>
#include <gdt.h>
#include <pic.h>
#include <arch.h>
#include <limine.h>
#include <tty.h>
#include <cpuinfo.h>

#include <fonts/dina_7x16.h>
#include <fonts/dina_bold_7x16.h>
#include <palettes/xubunterm.h>

// Will cause page fault or undefined behaviour with fonts bigger than 8 pixel width
// Also with resolutions above 1920x1080
#define TTY_BUFFER_SIZE 275 * 155

uint8_t term_buffer_text[TTY_BUFFER_SIZE];
uint32_t term_buffer_bg[TTY_BUFFER_SIZE];
uint32_t term_buffer_fg[TTY_BUFFER_SIZE];
uint8_t term_buffer_attr[TTY_BUFFER_SIZE];

    struct limine_framebuffer_request framebuffer_request = {
        LIMINE_FRAMEBUFFER_REQUEST, 0, NULL};

void arch::Setup() {
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
    } else {
        struct limine_framebuffer *lmn_fb = framebuffer_request.response->framebuffers[0];

        tty::Framebuffer fb;

        fb.vram = static_cast<uint32_t *>(lmn_fb->address);
        fb.width = lmn_fb->width;
        fb.height = lmn_fb->height;
        fb.pitch = lmn_fb->pitch;
        fb.bpp = lmn_fb->bpp;
        fb.font = dina_7x16;

        if (tty::tty1.Init(fb, dina_7x16, dina_bold_7x16, xubunterm_palette, term_buffer_text,
                term_buffer_bg, term_buffer_fg, term_buffer_attr)) {
            krnl::Printf_ok("TTY1 Ready");
        } else {
            krnl::Printf_fail("No Framebuffer for Terminal");
        }
    }

    gdt::Load();
    gdt::SetSegments(GDT_CS_KERNEL, GDT_DS_KERNEL); // 64-bit code, 64-bit data

    idt::Init();
    krnl::Printf_ok("IDT Initialized");

    pic::Init();
    krnl::Printf_ok("PIC Master Initialized");
    krnl::Printf_ok("PIC Slave Initialized");

    memory::Init();
    krnl::Printf_ok("Memory setup done");

    krnl::Printf_ok("Architecture setup done");

    krnl::Printf_info("CPU Model: %s", cpuinfo::GetModelString());
    krnl::Printf_info("Has APIC: %s", cpuinfo::HasAPIC() ? "true" : "false");
}