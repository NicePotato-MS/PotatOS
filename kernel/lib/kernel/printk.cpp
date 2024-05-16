#include <stdint.h>
#include <stdio.h>

#include <kernel.h>
#include <serial.h>
#include <ansi.h>
#include <io.h>

#ifdef DEBUG
static int debug_putchar(const char *chars, void *, size_t size) {
    while (size--) {
        io::outb(0xE9, *chars++);
    }
    return 0;
}
#endif

void krnl::Printf_varg(const char *str, va_list va) {
    #ifdef DEBUG
    // Print to debug port
    format(debug_putchar, NULL, str, va);
    #endif
}

void krnl::Printf(const char *str, ...) {
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
}

void krnl::Printf_ok(const char *str, ...) {
    krnl::Printf("%s[   OK] ", ANSI_BRIGHT_GREEN); // this
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
    krnl::Printf("%s\n", ANSI_RESET); // this
}

void krnl::Printf_fail(const char *str, ...) {
    krnl::Printf("%s[ FAIL] ", ANSI_BRIGHT_RED);
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
    krnl::Printf("%s\n", ANSI_RESET);
}

void krnl::Printf_info(const char *str, ...) {
    krnl::Printf("%s[ INFO] ", ANSI_BRIGHT_CYAN);
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
    krnl::Printf("%s\n", ANSI_RESET);
}

void krnl::Printf_warn(const char *str, ...) {
    krnl::Printf("%s%s[ WARN] ", ANSI_YELLOW, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
    krnl::Printf("%s\n", ANSI_RESET);
}

void krnl::Printf_error(const char *str, ...) {
    krnl::Printf("%s%s[ERROR] ", ANSI_RED, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
    krnl::Printf("%s\n", ANSI_RESET);
}

void krnl::Printf_panic(const char *str, ...) {
    krnl::Printf("%s%s%s[PANIC] ", ANSI_BG_BRIGHT_RED, ANSI_BLACK, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    krnl::Printf_varg(str, va);
    va_end(va);
    krnl::Printf("%s\n", ANSI_RESET);
}