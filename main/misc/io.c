#include "io.h"
#include <stdint.h>
#define VGA_COLS 80
#define VGA_ROWS 25
#define VGA_MEM ((uint16_t *)0xB8000)

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

static uint16_t vga_getCursor(void) {
	uint16_t pos = 0;
	outb(0x3D4, 0xE);
	pos |= ((uint16_t) inb(0x3D5)) << 8;
	outb(0x3D4, 0xF);
	pos |= inb(0x3D5);
	return pos;
}

static void vga_setCursor(uint16_t pos) {
	outb(0x3D4, 0xE);
	outb(0x3D5, (pos >> 8) & 0xFF);
	outb(0x3D4, 0xF);
	outb(0x3D5, pos & 0xFF);
}

static void vga_scroll(void) {
	for (uint16_t pos = 0; pos < VGA_COLS * (VGA_ROWS - 1); pos++)
		VGA_MEM[pos] = VGA_MEM[pos + VGA_COLS];

	for (uint16_t pos = VGA_COLS * (VGA_ROWS - 1); pos < VGA_COLS * VGA_ROWS; pos++)
		VGA_MEM[pos] = ' ' | 0x700;
	vga_setCursor(VGA_COLS * (VGA_ROWS - 1));
}

void serial_puts(char *string) {
	while (*string)
		serial_putc(*string++);
}

void serial_puti(int num) {
	uint8_t buf[100];
	buf[99] = 0;
	int point = 98;
	while (num != 0) {
		buf[point--] = (num % 10) + 48;
		num /= 10;
	}
	serial_puts(buf + point + 1);
}

void vga_putc(uint8_t character) {
	uint16_t pos = vga_getCursor();
	if (pos / VGA_COLS >= VGA_ROWS) {
		vga_scroll();
		pos = vga_getCursor();
	}
	if (character == '\n')
		pos += VGA_COLS - (pos % VGA_COLS);
	else if (character == '\r')
		pos -= pos % VGA_COLS;
	else if (character == '\b') {
		if (pos % VGA_COLS)
			VGA_MEM[--pos] = 0x700 | ' ';
	}
	else {
		VGA_MEM[pos] = 0x700 | character;
		pos++;
	}
	if (pos / VGA_COLS >= VGA_ROWS) {
		vga_scroll();
		pos = vga_getCursor();
	}
	vga_setCursor(pos);
}

void vga_puts(char *string) {
	while (*string)
		vga_putc(*string++);
}

void vga_puti(int num) {
	uint8_t buf[100];
	buf[99] = 0;
	int point = 98;
	while (num != 0) {
		buf[point--] = (num % 10) + 48;
		num /= 10;
	}
	vga_puts(buf + point + 1);
}
