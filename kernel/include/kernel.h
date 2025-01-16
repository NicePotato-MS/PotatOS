#pragma once

#include <stdint.h>
#include <stdarg.h>

#include <errorcodes.h>

[[noreturn]] void halt(void);

namespace krnl {
    [[noreturn]] void Panic(uint32_t errorcode);
    void Printf(const char *str, va_list va);
    void Printf(const char *str, ...);
    void Printf_ok(const char *str, ...);
    void Printf_fail(const char *str, ...);
    void Printf_info(const char *str, ...);
    void Printf_warn(const char *str, ...);
    void Printf_error(const char *str, ...);
    void Printf_panic(const char *str, ...);
}