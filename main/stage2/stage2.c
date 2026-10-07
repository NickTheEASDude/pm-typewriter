#include "io.h"
#include <stdint.h>

void cstart(void) {
	vga_puts("Hellorld!\n");
	for (;;) {
		asm volatile ("hlt");
	}
}
