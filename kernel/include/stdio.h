#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>

/**
 * Formats a string and outputs to an input putchar function
 *
 * @param in_putchar is the putchar functions expected to follow
    const char *chars, void *args, size_t size
 * @param putchar_args are args to be passed to putchar function (pointers for
    example)
 * @param str_in is the string to be formatted
 * @param va are arguments to replace type specifiers with
 * @return 0 if success or -1 if error
 */
int format(void *in_putchar, void *putchar_args, const char* str_in, va_list va);

#ifdef __cplusplus
}
#endif