bits	32
global	setupUART
%include	"pic.mac"
%define	PORT	3F8h

setupUART:
	pout	0,PORT+1
	pout	80h,PORT+3
	pout	3,PORT
	pout	0,PORT+1
	pout	3,PORT+3
	pout	0C7h,PORT+2
	pout	0Bh,PORT+4
	pout	1,PORT+1
	ret
