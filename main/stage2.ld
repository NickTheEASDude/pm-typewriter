OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(start32)

SECTIONS {
	. = 0x7E00;
	.text : ALIGN(4) {
		KEEP(*(.text.start))
		*(.text .text.*)
	}

	.data : ALIGN(4) {
		*(.rodata .rodata.*)
		*(.data .data.*)
	}

	.bss : ALIGN(4) {
		bssStart = .;
		*(.bss)
		*(COMMON)
		bssEnd = .;
	}
	. = ALIGN(16);
	stack = .;
	. += 0x4000;
	stackTop = .;
	/DISCARD/ : {
		*(.comment)
		*(.note*)
		*(.eh_frame*)
	}
}
