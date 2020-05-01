;                   equ     ;
ARDS_ADDR           equ     0x8000

section .entry
global osloader_start
[BITS 16]
osloader_start:
    jmp _text_start

section .text
_text_start:
    cli
    xor ax, ax
    mov es, ax
    mov di, 0x8000
    call _get_meminfo
_loop:
    jmp _loop;


; \brief get memory infomation of the machine
; \param es:di is buffer, first 4 bytes is numbre of entries in list, the following it is ARDS entrie
ards_size           equ     24
_get_meminfo:
            xor bp, bp

            mov eax, 0xe820
            xor ebx, ebx
            mov ecx, ards_size 
            mov edx, 0x534d4150
            mov es, bx
            push di
            add di, 4
            mov [es:di + 20], dword 1       ; for ACPI 3.0 Extended Capibility, force it to be valid

            clc                             ; for later error detect, clear carry flag
            int 0x15
            jc meminfo_failed               ; being stuck here indicates failure on reading memory info    

            mov edx, 0x534d4150
            cmp edx, eax
            jne meminfo_failed              ; INT 0x15 returned eax is not 0x534d4150 'SMAP'
            
            cmp ebx, 0
            je meminfo_failed               ; get ebx as 0, implies list is only 1 entry long, we see it as an error
            jmp meminfo_entrycheck

    meminfo_loop:
            mov eax, 0xe820
            mov [es:di + 20], dword 1
            int 0x15
            jc meminfo_success
            mov edx, 0x534d4150
            
    meminfo_entrycheck:
            jcxz meminfo_skip               ; ecx is zero, skip this entry
            cmp cl, 20
            jbe meminfo_notext
            test byte [es:di + 20], 1
            je meminfo_skip                 ; ACPI 3.0 Extended 

    meminfo_notext:
            mov ecx, [es:di + 8]            ; length_low bitwise_or length_high, if it is zero, it is not a vaild entry, skip it.
            or ecx, [es:di + 12]
            jz meminfo_skip
            inc bp
            add di, 24

    meminfo_skip:
            test ebx, ebx                   ; if ebx is 0, we reached the last entry
            jne meminfo_loop

    meminfo_success:
            pop di
            mov [di], bp
            ret
    meminfo_failed:
            jmp meminfo_failed
 
; if ecx is zero, skip it
; if ebx is zero, end it
; if, after int 15h , carry flag is set, end it

