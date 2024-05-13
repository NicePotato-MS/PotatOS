#include <stdint.h>

#include <kernel.h>
#include <serial.h>
#include <ansi.h>


void krn_Printk_varg(const char *str, va_list va) {
    if (srl_COM1.initalized) { srl_WritefVarg(&srl_COM1, str, va); }
}

void krn_Printk(const char *str, ...) {
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
}

void krn_Printk_ok(const char *str, ...) {
    krn_Printk("%s[   OK] ", ANSI_BRIGHT_GREEN); // this
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
    krn_Printk("%s\n", ANSI_RESET); // this
}

void krn_Printk_fail(const char *str, ...) {
    krn_Printk("%s[ FAIL] ", ANSI_BRIGHT_RED);
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
    krn_Printk("%s\n", ANSI_RESET);
}

void krn_Printk_info(const char *str, ...) {
    krn_Printk("%s[ INFO] ", ANSI_BRIGHT_CYAN);
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
    krn_Printk("%s\n", ANSI_RESET);
}

void krn_Printk_warn(const char *str, ...) {
    krn_Printk("%s%s[ WARN] ", ANSI_YELLOW, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
    krn_Printk("%s\n", ANSI_RESET);
}

void krn_Printk_error(const char *str, ...) {
    krn_Printk("%s%s[ERROR] ", ANSI_RED, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
    krn_Printk("%s\n", ANSI_RESET);
}

void krn_Printk_panic(const char *str, ...) {
    krn_Printk("%s%s%s[PANIC] ", ANSI_BG_BRIGHT_RED, ANSI_BLACK, ANSI_BOLD);
    va_list va;
    va_start(va, str);
    krn_Printk_varg(str, va);
    va_end(va);
    krn_Printk("%s\n", ANSI_RESET);
}