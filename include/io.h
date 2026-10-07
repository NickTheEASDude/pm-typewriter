#ifndef IO_H
#define IO_H
#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

void serial_putc(uint8_t character);
void serial_puts(char *string);
void serial_puti(int num);

void vga_putc(uint8_t character);
void vga_puts(char *string);
void vga_puti(int num);

#endif
