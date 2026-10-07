#include "io.h"
#include <stdint.h>


uint8_t inb(uint16_t port) {
	uint8_t result;
	asm volatile ("inb %w1, %b0" : "=a" (result) : "Nd" (port));
	return result;
}

void outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %b0, %w1" :: "a" (val), "Nd" (port) : "memory");
}

void serial_putc(uint8_t character) {
	while (inb(0x3FD) == 0x20);
	outb(0x3F8, character);
}

void serial_puts(char *string) {
	while (*string)
		serial_putc(*string++);
}
