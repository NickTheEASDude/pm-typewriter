#include <stdint.h>
#include "io.h"

#define _INTR __attribute__((section(".text.interrupts")))

_INTR void c_exception8(void) {
	serial_puts("FATAL: CPU Exception, halting system.\n");
	asm volatile ("cli; hlt");
}

_INTR void c_irq1(void) {}
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
