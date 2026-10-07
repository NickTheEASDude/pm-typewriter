#include "io.h"
#include <stdint.h>

void cstart(void) {
	vga_puts("Hellorld!\n");
	vga_puti(58);
	for (;;) {
		asm volatile ("hlt");
	}
}
