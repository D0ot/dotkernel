section .entry
global osloader_start
[BITS 16]
osloader_start:
    jmp _text_start

section .text
_text_start:
    jmp _text_start

