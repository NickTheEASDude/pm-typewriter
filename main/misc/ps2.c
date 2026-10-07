#include <stdint.h>
#include "io.h"

void setupPS2(void) {
	outb(0x64, 0xAD);
	outb(0x64, 0xA7);
	while (inb(0x64) & 1)
		inb(0x60);

	outb(0x64, 0x20);
	uint8_t config = inb(0x60);
	config &= 0b10111100;
	outb(0x64, 0x60);
	outb(0x60, config);

	outb(0x64, 0xAA);
	if (inb(0x60) != 0x55) {
		serial_puts("WARNING: PS/2 self-test failed, keyboard will not work.\n");
		return;
	}
	outb(0x64, 0xAE);
	outb(0x64, 0x20);
	config = inb(0x60);
	config |= 1;
	outb(0x64, 0x60);
	outb(0x60, config);
}
