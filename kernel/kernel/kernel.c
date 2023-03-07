#include <stdio.h>
#include <stdlib.h>
#include <lowlevel.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	
	printf("Protected mode started\n32-bit Kernel loaded\nWelcome to PotatOS!\n");
}
