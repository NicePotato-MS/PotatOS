#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define CONTROL_CHARACTER '%'
#define CONTROL_CHARACTER_STR "%"

#define BUFFER_SIZE 144

#define SIGN_NONE 0

#define FLAGS_NONE 0
#define FLAGS_PAD_ZERO 0b1 // Add extra 0s to fill space
#define FLAGS_LEFT_ALIGN 0b10 
#define FLAGS_SHOW_SIGN 0b100 // Always show a '+' or '-' on a number
#define FLAGS_SPACE_SIGN 0b1000  // Leave space where a '+' would be
#define FLAGS_ALTERNATE 0b10000   // Alternate Form flag (e.g. add 0x to %x)
#define FLAGS_SIGNED 0b100000     // Signed value
#define FLAGS_GROUPED 0b1000000  // Unused flag

static int putchar_noop(const char *, void *, size_t) { return 0; }

static int format_atoi(const char **str) {
    int val;
    for (val = 0; '0' <= **str && **str <= '9'; ++*str) {
        val *= 10;
        val += **str - '0';
    }
    return val;
}

#define putone(chr) \
    if (putchar(chr, putchar_args, 1) == -1) { return -1; }

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
int format(int (*in_putchar)(const char *, void *, size_t),
    void *putchar_args, const char *str_in, va_list va) {
    int (*putchar)(const char *, void *, size_t);
    putchar = in_putchar ? in_putchar : putchar_noop;

    const char *alphabet;
    unsigned int total_chars;
    int flags, width;
    uint8_t log2base;
    uint64_t sign_bit;

    char *str;

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
        flags = FLAGS_NONE;
    gather_flags:
        switch (*str_in++) {
            case '0': flags |= FLAGS_PAD_ZERO; goto gather_flags;
            case '-': flags |= FLAGS_LEFT_ALIGN; goto gather_flags;
            case '+': flags |= FLAGS_SHOW_SIGN; goto gather_flags;
            case ' ': flags |= FLAGS_SPACE_SIGN; goto gather_flags;
            case '#': flags |= FLAGS_ALTERNATE; goto gather_flags;
            default: str_in--; break;
        }

        // flags

        width = 0;
        if (isdigit(*str_in)) {
            width = format_atoi(&str_in);
        } else if (*str_in == '*') {
            width = va_arg(va, int);
            if (width < 0) {
                flags |= FLAGS_LEFT_ALIGN;
                width = -width;
            }
            str_in++;
        }

        // size

        sign_bit = (1ul << 31);
        switch (*str_in) {
            case 'h':
                str_in++;
                if (*str_in == 'h') {
                    str_in++;
                    sign_bit = (1ul << 7);
                } else {
                    sign_bit = (1ul << 15);
                }
                break;
            case 'l':
            case 't':  // ptrdiff_t
            case 'z':  // size_t
            case 'Z':  // size_t
                str_in++;
                sign_bit = (1ul << 63);
                break;
            default: break;
        }

        // format

        alphabet = "0123456789abcdefpx";
        log2base = 0;

        bool negative;
        int total;
        unsigned int padding_zeros;
        int len;

        char null_str[] = "(null)";

        switch (*str_in++) {
            case 'p':
                flags |= FLAGS_ALTERNATE;
                log2base = 4;
                sign_bit = 63;
                goto format_number;
            case 'X':
                alphabet = "0123456789ABCDEFPx";
                // fallthrough
            case 'x': log2base = 4; goto format_number;
            case 'b':
                alphabet = "0123456789abcdefpb";
                log2base = 1;
                goto format_number;
            case 'o': log2base = 3; goto format_number;
            case 'd':
            case 'i':
                flags |= FLAGS_SIGNED;
                // fallthrough
            case 'u':
                uint64_t value;
                flags &= ~FLAGS_ALTERNATE;
            format_number:
                value = va_arg(va, uint64_t);

                if (flags & FLAGS_LEFT_ALIGN) { flags &= ~FLAGS_PAD_ZERO; }
                if (!(flags & FLAGS_SIGNED)) {
                    flags &= ~(FLAGS_SHOW_SIGN | FLAGS_SPACE_SIGN);
                }

                negative = false;
                if (flags & FLAGS_SIGNED) {
                    if (value == sign_bit) {
                        negative = false;
                    } else {
                        if (value & sign_bit) {  // If negative
                            value = ~value + 1;  // Make positive
                            value &= sign_bit | (sign_bit - 1);
                            negative = true;
                        }
                    }
                }

                len = 0;
                unsigned int digit;
                char buffer[BUFFER_SIZE];
                char alternate_form_middle_char, sign_character;

                if (value) {
                    do {
                        if (!log2base) {
                            digit = value % 10;
                            value /= 10;
                        } else {
                            digit = value;
                            digit &= (1u << log2base) - 1;
                            value >>= log2base;
                        }
                        buffer[len++] = alphabet[digit];
                    } while (value);
                } else {
                    buffer[len++] = '0';
                }

                total = len;
                padding_zeros = 0;

                if (width && (flags & FLAGS_PAD_ZERO) &&
                    (negative ||
                        (flags & (FLAGS_SHOW_SIGN | FLAGS_SPACE_SIGN)))) {
                    width--;
                }
                if ((flags & FLAGS_PAD_ZERO) && (len < width)) {
                    padding_zeros += (width - len);
                    len = width;
                }
                if (flags & FLAGS_ALTERNATE) {
                    if (!(log2base == 3) && len && len >= width &&
                        (padding_zeros || buffer[len - 1] == '0')) {
                        len--;
                        if (len < total) { total = len; }
                        if (padding_zeros) { padding_zeros--; }
                        if (len && (log2base == 4 || log2base == 1) &&
                            (padding_zeros || buffer[len - 1] == '0')) {
                            if (padding_zeros) { padding_zeros--; }
                            len--;
                            if (len < total) { total = len; }
                        }
                    }
                    alternate_form_middle_char = 0;
                    if (log2base == 4 || log2base == 1) {
                        len++;
                        alternate_form_middle_char = alphabet[17];
                    }
                    len++;
                }
                sign_character = 0;
                if (negative) {
                    len++;
                    sign_character = '-';
                } else if (flags & FLAGS_SHOW_SIGN) {
                    len++;
                    sign_character = '+';
                } else if (flags & FLAGS_SPACE_SIGN) {
                    len++;
                    sign_character = ' ';
                }
                if (!(flags & FLAGS_PAD_ZERO) && !(flags & FLAGS_LEFT_ALIGN)) {
                    if (len < width) {
                        for (int i = 0; i < width - len; i++) { putone(" "); }
                    }
                }
                if (sign_character &&
                    putchar(&sign_character, putchar_args, 1) == -1) {
                    return -1;
                }
                if (flags & FLAGS_ALTERNATE) {
                    putone("0");
                    if (alternate_form_middle_char &&
                        putchar(&alternate_form_middle_char, putchar_args, 1) ==
                            -1) {
                        return -1;
                    }
                }
                for (size_t i = 0; i < padding_zeros; i++) { putone("0"); }
                // Reverse buffer
                for (int i = 0; i < total / 2; i++) {
                    char temp = buffer[i];
                    buffer[i] = buffer[total - i - 1];
                    buffer[total - i - 1] = temp;
                }
                putchar(buffer, putchar_args, total);
                if (flags & FLAGS_LEFT_ALIGN) {
                    if (len < width) {
                        for (int i = 0; i < width - len; i++) { putone(" "); }
                    }
                }

                break;
            case 's':
                str = va_arg(va, char *);
                if (!str) { str = null_str; }

                len = (int)strlen(str);

                if (!(flags & FLAGS_LEFT_ALIGN)) {
                    for (int i = 0; i < width - len; i++) { putone(" "); }
                }

                putchar(str, putchar_args, len);

                if (flags & FLAGS_LEFT_ALIGN) {
                    for (int i = 0; i < width - len; i++) { putone(" "); }
                }
                break;
            case CONTROL_CHARACTER:
                putone(CONTROL_CHARACTER_STR);
                break;
            case '\0':
                return -1;
            default: putchar(&str_in[-1], putchar_args, 1); break;
        }
    }
    return 0;
}
