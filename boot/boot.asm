%include "../arch/x86/gdt_helper.s"

; address
BOOTLOADER_BASE     equ     0x7c00
STACK_BASE          equ     0x7c00
OSLOADER_BASE       equ     0x7e00


; memory map in mbr and osloader stage
; 0x0500 - 0x7bff   : stack
; 0x7c00 - 0x7dff   : bootloader, mbr
; 0x7e00 - 0x7fff   : os loader 

; 0x80000 - 0xFFFFF ; unusable



; Bootloader starts here
[BITS 16]

global _start
_start:
section mbr
    jmp _text_start


section .text
_text_start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov sp, STACK_BASE

; clear screen
    mov ax, 0x600
    mov bx, 0x700
    mov cx, 0
    mov dx, 0x184f
    int 0x10

    mov ah, 0x03
    mov bh, 0x00
    int 0x10

; print a message
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
    call read_disk_realmode

    jmp 0x0:(OSLOADER_BASE)

; infite loop
loop:
    jmp loop


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


section .data

message db "Hello MBR"
message.size equ $-message