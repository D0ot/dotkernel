
extern osloader_main

section .text

[BITS 32]
global _pmode_start
_pmode_start:
    mov ax, (2 << 3)
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x7C00
    jmp osloader_main

