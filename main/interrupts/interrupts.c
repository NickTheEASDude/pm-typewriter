#include <stdint.h>
#include <stdbool.h>
#include "memory.h"
#include "io.h"

#define _INTR __attribute__((section(".text.interrupts")))

volatile uint8_t state = 0;

const char translation[0x80] = {
	[0x0E] = '`', [0x16] = '1', [0x1E] = '2', [0x26] = '3', [0x25] = '4',
	[0x2E] = '5', [0x36] = '6', [0x3D] = '7', [0x3E] = '8', [0x46] = '9',
	[0x45] = '0', [0x4E] = '-', [0x55] = '=', [0x66] = '\b', [0x0D] = '\t',
	[0x15] = 'Q', [0x1D] = 'W', [0x24] = 'E', [0x2D] = 'R', [0x2C] = 'T',
	[0x35] = 'Y', [0x3C] = 'U', [0x43] = 'I', [0x44] = 'O', [0x4D] = 'P',
	[0x54] = '[', [0x5B] = ']', [0x5A] = '\n', [0x5D] = '\\',
	[0x1C] = 'A', [0x1B] = 'S', [0x23] = 'D', [0x2B] = 'F', [0x34] = 'G',
	[0x33] = 'H', [0x3B] = 'J', [0x42] = 'K', [0x4B] = 'L', [0x4C] = ';',
	[0x52] = '\'',
	[0x1A] = 'Z', [0x22] = 'X', [0x21] = 'C', [0x2A] = 'V', [0x32] = 'B',
	[0x31] = 'N', [0x3A] = 'M', [0x41] = ',', [0x49] = '.', [0x4A] = '/',
	[0x29] = ' ',
};

_INTR void c_exception8(void) {
	serial_puts("FATAL: CPU Exception, halting system.\n");
	asm volatile ("cli; hlt");
}

_INTR void c_irq1(void) {
	while (inb(0x64) & 1) {
		uint8_t scancode = inb(0x60);
		switch (state) {
			case 0:
				if (scancode == 0xE0)
					state = 1;
				else if (scancode == 0xF0)
					state = 2;
				else {
					uint8_t character = translation[scancode];
					if (character)
						vga_putc(character);
					state = 0;
				}
				break;
			case 1:
				if (scancode == 0xF0)
					state = 2;
				state = 0;
				break;
			case 2:
				state = 0;
				break;
			default:
				state = 0;
				break;
		}
	}
	outb(0x80, 0);
}
_INTR void c_irq4(void) {
	uint8_t charReceived = inb(0x3F8);
	if (charReceived == 8 || charReceived == 0x7F) {
		serial_putc(8);
		serial_putc(' ');
		serial_putc(8);
	} else if (charReceived == '\r') {
		serial_putc('\r');
		serial_putc('\n');
	} else
		serial_putc(charReceived);
}
