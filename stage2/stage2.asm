bits	32
org	7E00h

SECBUF	equ	9000h
DEST	equ	100000h
START_LBA equ	3

start32:
	cld
	mov	esp,7E00h
	mov	ax,ds
	mov	es,ax

	mov	eax,START_LBA
	mov	bl,1
	mov	edi,SECBUF
	call	read_sectors

	movzx	ebp,word [SECBUF+2]
	mov	eax,START_LBA
	mov	edi,DEST

.batch:
	test	ebp,ebp
	jz	.done
	mov	ebx,ebp
	cmp	ebx,255
	jbe	.ok
	mov	ebx,255
.ok:	sub	ebp,ebx
	push	eax
	push	ebx
	call	read_sectors
	pop	ebx
	pop	eax
	add	eax,ebx	
	jmp	.batch

.done:	jmp	DEST

read_sectors:
	mov	esi,eax

	mov	dx,1F7h
.busy:	in	al,dx
	test	al,80h
	jnz	.busy

	mov	dx,1F6h
	mov	eax,esi
	shr	eax,24
	and	al,0Fh
	or	al,0E0h
	out	dx,al

	mov	dx,1F2h
	mov	al,bl
	out	dx,al

	mov	dx,1F3h
	mov	eax,esi
	out	dx,al
	inc	dx
	shr	eax,8
	out	dx,al
	inc	dx
	shr	eax,8
	out	dx,al

	mov	dx,1F7h
	mov	al,20h
	out	dx,al

.sector:
	mov	dx,1F7h
	in	al,dx
	in	al,dx
	in	al,dx
	in	al,dx
.drq:	in	al,dx
	test	al,80h
	jnz	.drq
	test	al,21h
	jnz	.error
	test	al,08h
	jz	.drq

	mov	dx,1F0h
	mov	ecx,256
	rep	insw
	dec	bl
	jnz	.sector
	ret

.error:	jmp	$

times	1024-($-start32)	db 0	; pad to 2 sectors
