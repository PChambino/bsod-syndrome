CPU 686
BITS 32

%define EOI 20h
%define PIC1_CMD 20h
%define DATA_REG 60h

global _kbd_isr
extern _queuePut
extern _keys

section .text
_kbd_isr:
	push eax ; todos os registos utilizados na rotina
	; instru��es da rotina

	mov eax, 0
	in al, DATA_REG
	
	push eax
	push dword _keys
	call _queuePut
	add esp, 8

	mov al, EOI ; sinaliza EOI para o PIC-1
	out PIC1_CMD, al

	pop eax ; todos os registos guardados
	iretd
END