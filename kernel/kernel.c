#include <stdint.h>
#include <stddef.h>
#include <limine.h>

// DO NOT REMOVE STRING.H, THINGS *WILL* BREAK
#include <string.h>
#include <lowlevel.h>
#include <pcspeaker.h>

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

// Halt and catch fire function.
static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void) {
    // Ensure we got a terminal
    if (terminal_request.response == NULL
     || terminal_request.response->terminal_count < 1) {
        hcf();
    }

    // We should now be able to call the Limine terminal to print out
    // a simple "Hello World" to screen.
    const char *hello_msg = "Hello World";

    struct limine_terminal *terminal = terminal_request.response->terminals[0];
    terminal_request.response->write(terminal, hello_msg, strlen(hello_msg));

    // We're done, just hang...
    hcf();
}
