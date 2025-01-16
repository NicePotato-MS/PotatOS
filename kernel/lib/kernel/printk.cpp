#include <stdint.h>
#include <stdio.h>

#include <kernel.h>
#include <serial.h>
#include <ansi.h>
#include <tty.h>
#include <io.h>

#ifdef DEBUG
static int putchar_dbg_noterm(const char *chars, void *, size_t size) {
    while (size--) {
        io::outb(0xE9, *chars);
        chars++;
    }
    return 0;
}

static int putchar_dbg(const char *chars, void *, size_t size) {
    while (size--) {
        io::outb(0xE9, *chars);
        tty::tty1.Putchar((uint8_t)*chars);
        chars++;
    }
    return 0;
}
#else
static int putchar(const char *chars, void *, size_t size) {
    while (size--) {
        tty::tty1.Putchar((uint8_t)*chars);
        chars++;
    }
    return 0;
}
#endif

void krnl::Printf(const char *str, va_list va) {
    if (tty::tty1.initialized) {
        #ifdef DEBUG
        format(putchar_dbg, NULL, str, va);
        #else
        format(putchar, NULL, str, va);
        #endif
    } else {
        #ifdef DEBUG
        format(putchar_dbg_noterm, NULL, str, va);
        #endif
    }
    
}

void krnl::Printf(const char *str, ...) {
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
}

void krnl::Printf_ok(const char *str, ...) {
    krnl::Printf("%s[   OK] ", ansi::fg::bright_green); // this
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
    krnl::Printf("%s\n", ansi::reset); // this
}

void krnl::Printf_fail(const char *str, ...) {
    krnl::Printf("%s[ FAIL] ", ansi::fg::bright_red);
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
    krnl::Printf("%s\n", ansi::reset);
}

void krnl::Printf_info(const char *str, ...) {
    krnl::Printf("%s[ INFO] ", ansi::fg::bright_cyan);
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
    krnl::Printf("%s\n", ansi::reset);
}

void krnl::Printf_warn(const char *str, ...) {
    krnl::Printf("%s%s[ WARN] ", ansi::fg::yellow, ansi::bold);
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
    krnl::Printf("%s\n", ansi::reset);
}

void krnl::Printf_error(const char *str, ...) {
    krnl::Printf("%s%s[ERROR] ", ansi::fg::red, ansi::bold);
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
    krnl::Printf("%s\n", ansi::reset);
}

void krnl::Printf_panic(const char *str, ...) {
    krnl::Printf("%s%s%s[PANIC] ", ansi::bg::bright_red, ansi::fg::black, ansi::bold);
    va_list va;
    va_start(va, str);
    krnl::Printf(str, va);
    va_end(va);
    krnl::Printf("%s\n", ansi::reset);
}