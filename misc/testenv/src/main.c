#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include <fmt.h>

int test_putchar(const char *str, void *, size_t size) {
    fwrite(str, sizeof(char), size, stdout);
    return 0;
}

int printf_test(const char *str, ...) {
    va_list va;
    va_start(va, str);
    uint8_t ret = format(test_putchar, NULL, str, va);
    va_end(va);
    return ret;
}

int main() {
    uint8_t error = 0xFE;
    printf_test("%s %#08X Test\n", "hello", error);
    
    return 0;
}