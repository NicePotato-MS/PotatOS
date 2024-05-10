#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#define CONTROL_CHARACTER '%'

#define SIGN_NONE 0

#define FLAGS_NONE 0
#define FLAGS_PAD_ZERO 0b1 // Add extra 0s to fill space
#define FLAGS_LEFT_ALIGN 0b1 << 1
#define FLAGS_SHOW_SIGN 0b1 << 2 // Always show a '+' or '-' on a number
#define FLAGS_SPACE_SIGN 0b1 << 3  // Leave space where a '+' would be
#define FLAGS_ALTERNATE 0b1 << 4   // Alternate Form flag (e.g. add 0x to %x)

static int putchar_noop(void) { return 0; }

#define __putchar(chr)                                              \
    do {                                                            \
        char out_buffer[sizeof(chr)] = {chr};                       \
        if (putchar(out_buffer, putchar_args, sizeof(chr)) == -1) { \
            return -1;                                              \
        }                                                           \
    } while (0) // do while adds another scope for out_buffer

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
int format(void *in_putchar, void *putchar_args, const char* str_in, va_list va) {
    int (*putchar)(const char *, void *, size_t);
    putchar = in_putchar ? in_putchar : (void *)putchar_noop;

    int total_chars;

    const char *alphabet;
    unsigned char sign, sign_bit;
    int flags, width;
    
    while (*str_in) {
        if (*str_in != CONTROL_CHARACTER) {
            for (total_chars = 1; str_in[total_chars]; ++total_chars) {
                if (str_in[total_chars] == CONTROL_CHARACTER) { break; }
            }
            if (putchar(str_in, putchar_args, total_chars) == -1) { return -1; }
            str_in += total_chars;
            continue;
        }

        str_in++;
        sign = SIGN_NONE;
        flags = FLAGS_NONE;
    gather_flags:
        switch (*str_in++) {
            case '0':
                flags |= FLAGS_PAD_ZERO;
                goto gather_flags;
            case '-':
                flags |= FLAGS_LEFT_ALIGN;
                goto gather_flags;
            case '+':
                flags |= FLAGS_SHOW_SIGN;
                goto gather_flags;
            case ' ':
                flags |= FLAGS_SPACE_SIGN;
                goto gather_flags;
            case '#':
                flags |= FLAGS_ALTERNATE;
                goto gather_flags;
            default:
                str_in--;
                break;
        }

        width = 0;
        if (isdigit(*str_in)) {
            width = 
        }
        
        switch (*str_in++) {
            case 'a':
                __putchar("(formatted)");
                break;
            case CONTROL_CHARACTER:
                __putchar(CONTROL_CHARACTER);
                break;
            default:

        }
    }
    return 0;
}
