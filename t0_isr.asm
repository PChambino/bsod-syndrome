CPU 686
BITS 32

%define EOI 20h
%define PIC1_CMD 20h

global _t0_isr
global _time_tick

section .data
_time_tick dd 0

section .text
_t0_isr:
	; instruções da rotina

	inc dword[_time_tick]

	mov al, EOI ; sinaliza EOI para o PIC-1
	out PIC1_CMD, al

	iretd
END