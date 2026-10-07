#ifndef IO_H
#define IO_H
#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

void serial_putc(uint8_t character);
void serial_puts(char *string);

#endif
