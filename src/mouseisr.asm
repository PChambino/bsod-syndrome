CPU 686
BITS 32

%define EOI 20h
%define PIC1_CMD 20h
%define PIC2_CMD 0xA0
%define DATA_REG 60h

global _mouse_isr
global _mouse_data
global _mouse_data_i
;global _mouse_flag

global _mouse_isr_end

extern _mouseQueue
extern _putGQueue

section .data
_mouse_data db 0,0,0
_mouse_data_i dd 0
;_mouse_flag dd 0

section .text
_mouse_isr:
	push eax
	push ecx
	; todos os registos utilizados na rotina
	; instruções da rotina

	mov eax, 0
	
	mov ecx, dword[_mouse_data_i]
	mov eax, 0
	in al, DATA_REG
	mov byte[_mouse_data + ecx], al
	
	inc ecx
	mov dword[_mouse_data_i], ecx
	
	cmp ecx, 3
	jne fim
		mov ecx, 0
		mov dword[_mouse_data_i], ecx
		;mov dword[_mouse_flag], 1
		push _mouse_data
		push dword[_mouseQueue]
		call _putGQueue
		add esp, 8
	fim:

	mov eax, 0
	
	mov al, EOI ; sinaliza EOI para o PIC-1
	out PIC1_CMD, al
	out PIC2_CMD, al
	
	pop ecx
	pop eax ; todos os registos guardados
	iretd

_mouse_isr_end:
	
END
