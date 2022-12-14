#include <stdint.h>
#include <conio.h>

typedef unsigned char* uchar;

void nmemcpy(void *dest, void *src, uint16_t n)
{
// Typecast src and dest addresses to (char *)
char *csrc = (char *)src;
char *cdest = (char *)dest;
 
// Copy contents of src[] to dest[]
for (int i=0; i<n; i++)
    cdest[i] = csrc[i];
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}