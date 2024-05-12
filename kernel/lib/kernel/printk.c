#include <stdint.h>

#include <kernel.h>
#include <serial.h>
#include <ansi.h>


void printk_varg(const char *str, va_list va) {
    if (srl_COM1.initalized) { srl_WritefVarg(&srl_COM1, str, va); }
}

void printk(const char *str, ...) {
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
}

void printk_ok(const char *str, ...) {
    printk("%s[   OK] ", ANSI_BRIGHT_GREEN);
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
    //printk("%s\n", ANSI_RESET);
}

void printk_fail(const char *str, ...) {
    printk("%s[ FAIL] ", ANSI_BRIGHT_RED);
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
    printk("%s\n", ANSI_RESET);
}

void printk_info(const char *str, ...) {
    printk("%s[ INFO] ", ANSI_BRIGHT_CYAN);
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
    printk("%s\n", ANSI_RESET);
}

void printk_warn(const char *str, ...) {
    printk("%s%s[ WARN] ", ANSI_YELLOW, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
    printk("%s\n", ANSI_RESET);
}

void printk_error(const char *str, ...) {
    printk("%s%s[ERROR] ", ANSI_RED, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
    printk("%s\n", ANSI_RESET);
}

void printk_panic(const char *str, ...) {
    printk("%s%s%s[PANIC] ", ANSI_BG_BRIGHT_RED, ANSI_BLACK, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    printk_varg(str, va);
    va_end(va);
    printk("%s\n", ANSI_RESET);
}