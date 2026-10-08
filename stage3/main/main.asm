bits	32
global	kstart
global	diskSize
extern	sectorCount
extern	cstart
extern	stackTop
extern	setupPIC
extern	setupUART
extern	setupIDT
extern	setupPS2
extern	bssStart
extern	bssEnd

section .data
gdt:	dq	0
	dw	0FFFFh
	dw	0
	db	0
	db	10011010b
	db	11001111b
	db	0
	dw	0FFFFh
	dw	0
	db	0
	db	10010010b
	db	11001111b
	db	0
gdtDesc:dw	gdtDesc - gdt - 1
	dd	gdt

section	.text.start
kstart:
	jmp	entry
diskSize:
	dw	sectorCount
entry:	lgdt	[gdtDesc]
	jmp	08h:.reload
.reload:
	mov	ax,10h
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	lea	esp,DWORD[stackTop]
	mov	edi,bssStart
	mov	ecx,bssEnd
	sub	ecx,edi
	xor	eax,eax
	cld
	rep	stosb
	
	call	setupPIC
	call	setupUART
	call	setupPS2
	call	setupIDT
	sti

	cld
	call	cstart
	
	cli
loop:	hlt
	jmp	loop
