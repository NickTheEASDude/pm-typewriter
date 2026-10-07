#include "io.h"
#include <stdint.h>

void cstart(void) {
	asm volatile ("cli");
	serial_puts("\nHellorld!\n");
	asm volatile ("sti");
	for (;;) {
		asm volatile ("hlt");
	}
}
