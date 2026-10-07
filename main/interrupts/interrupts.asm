bits	32
global	setupPIC
global	setupIDT
extern	serial_putc
extern	serial_puts
%include	"pic.mac"

section	.bss
align	16
idt:	resq	256

section	.data
idtDesc:
	dw	idt + 255
	dd	idt

section	.text.interrupts
%macro CENTRY 2
extern	c_%1
%1:	pushad
	call	c_%1
	mov	eax,%2
	jz	.skip
	pout	20h,PIC1_COMMAND
.skip:
	popad
	iret
%endmacro

exception0:
	enter	0,0
	pushad
	push	.msg
	call	serial_puts
	add	esp,4
	add	DWORD[ebp+4],1
	popad
	leave
	iret
.msg:	db	"ERROR: CPU Exception (division error), skipping.",10,0
CENTRY	exception8,0
CENTRY	irq1,1
CENTRY	irq4,1

section	.text
%macro addID 3
	mov	edi,idt+(%2*8)
	lea	eax,%1
	mov	WORD[edi],ax
	mov	WORD[edi+2],8h
	mov	BYTE[edi+4],0
	mov	BYTE[edi+5],80h|%3
	shr	eax,16
	mov	WORD[edi+6],ax
%endmacro

setupPIC:
	pout	ICW1_INIT|ICW1_ICW4,PIC1_COMMAND
	io_wait
	pout	ICW1_INIT|ICW1_ICW4,PIC2_COMMAND
	io_wait
	pout	20h,PIC1_DATA
	io_wait
	pout	28h,PIC2_DATA
	io_wait
	pout	1<<CASCADE_IRQ,PIC1_DATA
	io_wait
	pout	CASCADE_IRQ,PIC2_DATA
	io_wait
	pout	ICW4_8086,PIC1_DATA
	io_wait
	pout	ICW4_8086,PIC2_DATA
	io_wait
	pout	0EFh,PIC1_DATA
	pout	0FFh,PIC2_DATA
	ret

setupIDT:
	push	eax
	push	edi
	addID	exception0,0h,0Fh
	addID	exception8,8h,0Fh
	addID	irq1,21h,0Eh
	addID	irq4,24h,0Eh
	lidt	[idtDesc]
	pop	eax
	pop	edi
	ret
