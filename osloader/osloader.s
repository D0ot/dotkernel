%include "../arch/x86/gdt_helper.s"

;                   equ     ;
ARDS_ADDR           equ     0x7e00 + 0x1400

extern osloader_main

[BITS 16]

section .text
global _osloader_start:function
_osloader_start:
    jmp text_start

text_start:
    xor ax, ax
    mov es, ax
    mov di, ARDS_ADDR
    call get_meminfo


; disable interrupts
    cli

; to enable Protected Mode
; open A20
    in al, 0x92
    or al, 0000_0010b
    out 0x92, al

; set GDT
    lgdt [gdt_ptr]

; set PE bit
    mov eax, cr0
    or eax, 1b
    mov cr0, eax
    jmp dword (1 << 3):_flush


[BITS 32]
_flush:
    mov ax, (2 << 3)
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x7c00
    call osloader_main
_inf_loop:
    jmp _inf_loop




; \brief get memory infomation of the machine
; \param es:di is buffer, first 4 bytes is numbre of entries in list, the following it is ARDS entrie
[BITS 16]
ards_size           equ     24
get_meminfo:
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




; content about gdt
section .gdt_data

; flat 4G
gdt_addr:

GDT_BASE            dd      0x0
                    dd      0x0

CODE_DESC           dd      0x0000ffff
                    dd      (0 << DESC_BASE_OFFSET2)+ \
                            (0xf << DESC_BASE_OFFSET1) + \
                            DESC_TYPE_CODE_EX + \
                            DESC_S_USR + \
                            DESC_DPL_0 + \
                            DESC_P_SET + \
                            DESC_L_CLS + \
                            DESC_DB_B + \
                            DESC_G_4K + \
                            (0 << DESC_BASE_OFFSET3)

DATA_DESC           dd      0x0000ffff
                    dd      (0 << DESC_BASE_OFFSET2)+ \
                            (0xf << DESC_BASE_OFFSET1) + \
                            DESC_TYPE_DATA_RDWR + \
                            DESC_S_USR + \
                            DESC_DPL_0 + \
                            DESC_P_SET + \
                            DESC_L_CLS + \
                            DESC_DB_B + \
                            DESC_G_4K + \
                            (0 << DESC_BASE_OFFSET3)

GDT_SIZE            equ     $ - GDT_BASE
GDT_LIMIT           equ      GDT_SIZE - 1

gdt_ptr:            dw      GDT_LIMIT
                    dd      gdt_addr
