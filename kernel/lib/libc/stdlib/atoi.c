#include <stdlib.h>

int atoi(const char *nptr) {
    int x = 0, d = 1;
    char c;

    while (*nptr == ' ' || *nptr == '\t' || *nptr == '\n')
        nptr++;

    if (*nptr == '-') {
        d = -1;
        nptr++;
    } else if (*nptr == '+') {
        nptr++;
    }

    // Loop through each digit in string
    while ((c = *nptr++) != '\0') {
        if (c >= '0' && c <= '9') {
            x *= 10;
            x += (c - '0') * d;
        } else {
            break;  // Break on non-digit
        }
    }

    return x;
}