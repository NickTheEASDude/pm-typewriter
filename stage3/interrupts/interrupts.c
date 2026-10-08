#include <stdint.h>
#include <stdbool.h>
#include "scancodes.h" // translation, translationUpper, translationCaps, translationUpper
#include "memory.h"
#include "io.h"

#define _INTR __attribute__((section(".text.interrupts")))

typedef enum {
	STATE_NORMAL,
	STATE_RELEASE,
	STATE_EXTEND,
	STATE_EXTEND_RELEASE
} kbdState_t;

static kbdState_t state = STATE_NORMAL;


static const uint8_t *transPoint = translation;
static bool lShift = false;
static bool rShift = false;
static bool caps = false;

_INTR void c_exception8(void) {
	serial_puts("FATAL: CPU Exception, halting system.\n");
	asm volatile ("cli; hlt");
}

_INTR void c_irq1(void) {
	while (inb(0x64) & 1) {
		uint8_t scancode = inb(0x60);
		switch (state) {
			case STATE_NORMAL:
				if (scancode == 0xE0)
					state = STATE_EXTEND;
				else if (scancode == 0xF0)
					state = STATE_RELEASE;
				else if (scancode == 0x58) {
					caps = !caps;
					if (lShift || rShift)
						transPoint = (caps ? translationCapsUpper : translationUpper);
					else
						transPoint = (caps ? translationCaps : translation);
				} else {
					if (scancode == 0x12 || scancode == 0x59) {
						if (scancode == 0x12)
							lShift = true;
						if (scancode == 0x59)
							rShift = true;
						transPoint = (caps ? translationCapsUpper : translationUpper);
					}
					uint8_t character = transPoint[scancode];
					if (character)
						vga_putc(character);
					state = STATE_NORMAL;
				}
				break;

			case STATE_RELEASE:
				if (scancode == 0x12)
					lShift = false;
				if (scancode == 0x59)
					rShift = false;
				if (!lShift && !rShift)
					transPoint = (caps ? translationCaps : translation);
				state = STATE_NORMAL;
				break;

			case STATE_EXTEND:
				if (scancode == 0xF0)
					state = STATE_EXTEND_RELEASE;
				else
					state = STATE_NORMAL;
				break;
			
			case STATE_EXTEND_RELEASE:
				state = STATE_NORMAL;
				break;
		}
	}
}
_INTR void c_irq4(void) {
	uint8_t charReceived = inb(0x3F8);
	if (charReceived == '\r')
		serial_putc('\n');
	else if (charReceived == 8 || charReceived == 0x7F) {
		serial_putc(8);
		serial_putc(' ');
		serial_putc(8);
	} else
		serial_putc(charReceived);
}
