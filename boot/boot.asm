org	7C00h
bits	16

entry:
	jmp	short	start16
	nop
times	87	db	0
start16:
	cli
	mov	ax,0
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	mov	ax,7C00h
	mov	sp,ax
	sti
.retry:	mov	si,disk
	mov	ah,42h
	mov	dl,80h
	stc
	int	13h
	jc	.retry
	cli
	lgdt	[gdtDesc]
	mov	eax,cr0
	or	al,1
	mov	cr0,eax
	jmp	08h:(7C00h + (start32 - entry))

bits	32
start32:
	mov	ax,10h
	mov	ds,ax
	mov	es,ax
	mov	ss,ax
	jmp	7E00h

disk:	db	10h
	db	0
	dw	2
	dw	7E00h
	dw	0
	dd	01
	dd	0

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
pad:	times	510-($-entry)	db	0
	dw	0AA55h
