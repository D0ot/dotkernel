%include "gdt_helper.s"

; address
BOOTLOADER_BASE     equ     0x7c00
STACK_BASE          equ     0x7c00
OSLOADER_BASE       equ     0x8000


; memory map
; 0x0500 - 0x7bff   : stack
; 0x7c00 - 0x7dff   : bootloader, mbr
; 0x7e00 - 0x8000   : initial gdt table
; 0x8000 - 0x9000   : os loader 

; 0x80000 - 0xFFFFF ; unusable



; Bootloader starts here
[BITS 16]
section mbr vstart=BOOTLOADER_BASE
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov sp, STACK_BASE

    mov ax, 0x600
    mov bx, 0x700
    mov cx, 0
    mov dx, 0x184f
    int 0x10

    mov ah, 0x03
    mov bh, 0x00
    int 0x10

    mov ax, message
    mov bp, ax
    mov cx, message.size
    mov ax, 0x1301
    mov bx, 0x02
    int 0x10

; load osloader
; osloader is close to mbr
    mov eax, 0x01
    mov bx, OSLOADER_BASE
    mov cx, 10 ; 10 is enough
    ;call read_disk_realmode

; open A20
    in al, 0x92
    or al, 0000_0010b
    out 0x92, al

; set GDT
    lgdt [gdt_addr]

; set PE bit
    mov eax, cr0
    or eax, 1b
    mov cr0, eax

    jmp $
    jmp dword (1 << 3):pmode_start


[BITS 32]
pmode_start:


[BITS 16]
; eax : LBA 
; bx : buffer
; cx : sector count
read_disk_realmode:
    mov esi, eax
    mov di, cx

    mov dx, 0x1f2
    mov al, cl
    out dx, al ; al = where to read
    mov eax, esi

    mov dx, 0x1f3
    out dx, al

    mov cl, 8
    shr eax, cl
    mov dx, 0x1f4
    out dx, al

    shr eax, cl
    mov dx, 0x1f5
    out dx, al

    shr eax, cl
    and al, 0x0f
    or al, 0xe0
    mov dx, 0x1f6
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    .not_ready:
    nop
    in al, dx
    and al, 0x88
    cmp al, 0x08
    jnz .not_ready

    mov ax, di
    mov dx, 256
    mul dx
    mov cx, ax
    mov dx, 0x1f0
    .go_on_read:
    in ax, dx
    mov [bx], ax
    add bx, 2
    loop .go_on_read
    ret

; global descriptor 
; a data descriptor and a code descriptor


; flat 4G
gdt_addr:

GDT_BASE            dd      0x0
                    dd      0x0

CODE_DESC           dd      0x0000ffff
                    dd      (0 << DESC_BASE_OFFSET2)+ \
                            DESC_TYPE_CODE_EX + \
                            DESC_S_SYS + \
                            DESC_DPL_0 + \
                            DESC_P_SET + \
                            DESC_L_CLS + \
                            DESC_G_4K + \
                            (0 << DESC_BASE_OFFSET3)

DATA_DESC           dd      0x0000ffff
                    dd      (0 << DESC_BASE_OFFSET2)+ \
                            DESC_TYPE_DATA_RDWR + \
                            DESC_S_SYS + \
                            DESC_DPL_0 + \
                            DESC_P_SET + \
                            DESC_L_CLS + \
                            DESC_G_4K + \
                            (0 << DESC_BASE_OFFSET3)

GDT_SIZE            equ     $ - GDT_BASE
GDT_LIMIT           dd      GDT_SIZE - 1

gdt_ptr:            dw      GDT_LIMIT
                    dd      gdt_addr




    message db "Hello MBR"
    message.size equ $-message
    times 510-($-$$) db 0
    db 0x55, 0xaa