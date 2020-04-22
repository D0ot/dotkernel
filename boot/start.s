; Bootloader starts here

[BITS 16]
section mbr vstart=0x7c00
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov sp, 0x7c00

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

    mov eax, 0x02
    mov bx, 0x8000
    mov cx, 1
    call read_disk

    jmp $
; eax : LBA 
; bx : buffer
; cx : sector count
read_disk:
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




    message db "Hello MBR"
    message.size equ $-message
    times 510-($-$$) db 0
    db 0x55, 0xaa