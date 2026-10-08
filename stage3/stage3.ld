OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(kstart)

SECTIONS {
	. = 0x100000;
	.text : ALIGN(16) {
		KEEP(*(.text.start))
		*(.text .text.*)
	}
	.rodata : ALIGN(16) {
		*(.rodata .rodata.*)
	}
	.data : ALIGN(16) {
		*(.data .data.*)
		. = ALIGN(512);
	}
	sectorCount = ABSOLUTE((. - 0x100000) / 512);
	.bss : ALIGN(16) {
		bssStart = .;
		. = ALIGN(4);
		*(.bss .bss.*)
		*(COMMON)
		bssEnd = .;
	}
	. = ALIGN(16);
	stack = .;
	. += 0x100000;
	stackTop = .;
	/DISCARD/ : {
		*(.comment)
		*(.note*)
		*(.eh_frame*)
	}
}
