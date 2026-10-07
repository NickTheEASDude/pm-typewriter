bits	32
global	start32
extern	cstart
extern	stackTop
extern	setupPIC
extern	setupUART
extern	setupIDT
extern	setupPS2
extern	bssStart
extern	bssEnd

section	.text.start
start32:
	mov	edi,bssStart
	mov	ecx,bssEnd
	sub	ecx,edi
	xor	eax,eax
	cld
	rep	stosb
	lea	esp,DWORD[stackTop]
	
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
