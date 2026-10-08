#include "io.h"
#include <stdint.h>

void cstart(void) {
	vga_puts("Hellorld!\n");
	serial_puts("\nHellorld!\n");
	for (;;) {
		asm volatile ("hlt");
	}
}
