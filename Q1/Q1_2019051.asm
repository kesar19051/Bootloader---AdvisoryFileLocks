[bits 16]
[org 0x7c00]

boot:
	mov ax, 0x2401
	int 0x15
	mov ax, 0x3
	int 0x10
	cli
	lgdt [gdt_pointer]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:boot32
	
gdt_start:
	dq 0x0
	
gdt_code:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0
	
gdt_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
	
gdt_end:
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start
	
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[bits 32]
boot32:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esi, hello
	mov ebx, 0xb8000
	mov edx, cr0
	mov ecx, 32
	mov ebx, 000B8000h
	
.loop1:
	mov eax, 00000130h
	shl edx, 1
	adc eax, 0
	mov [ebx], ax
	add ebx, 2
	dec ecx
	jnz .loop1

.loop:
	lodsb
	or al, al
	jz haltt
	or eax, 0x0100
	mov word [ebx], ax
	add ebx, 2
	jmp .loop
	
haltt:
	hlt
	
hello: db "  Hello World!", 0

times 510 - ($-$$) db 1
dw 0xaa55
