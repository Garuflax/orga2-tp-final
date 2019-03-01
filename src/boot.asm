; ** por compatibilidad se omiten tildes **
; ==================================================================================
; TRABAJO PRACTICO FINAL - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==================================================================================

	
BITS 16
		; Copiar kernel
		mov ax,0x0100
		mov ch,0;cylinder number
		mov cl,2;sector number
		mov dh,0;head number
		mov dl,0;drive number
		mov es,ax;segment
		mov bx,0x0000;offset
		mov al,0x20;sectors amount
		mov ah,2;read
		int 0x13
		; Copiar tareas
		mov ax,0x0800
		mov ch,0;cylinder number
		mov cl,16;sector number
		mov dh,1;head number
		mov dl,0;drive number
		mov es,ax;segment
		mov bx,0x0000;offset
		mov al,0x20;sectors amount
		mov ah,2;read
		int 0x13
		jmp 0x0000:0x1000;ubicación del kernel cargado
		hlt

	times (510-$+$$) db 0x00
	db 0x55,0xAA