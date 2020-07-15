%include "../arch/x86/gdt_helper.s"
%include "../defs/defs.s"

; address


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
    jmp 0x0:_text_start


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
    ;mov eax, 0x01
    ;mov bx, OSLOADER_BASE
    ;mov cx, 10 ; 10 is enough
    ;call read_disk_realmode

    mov edi, OSLOADER_BASE
    mov bl, 0x10
    mov dx, 0x1f0
    mov ebp, 1
    call pio28_read

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
    mov dx, 0x1f7
    in al, dx
    in al, dx
    in al, dx
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

;ATA PI0 28bit singletasking disk read function (up to 256 sectors)
; inputs: ESI -> driverdata info, EDI -> destination buffer
; BL = sectors to read, DX = base bus I/O port (0x1F0, 0x170, ...), EBP = 28bit "relative" LBA
; BSY and DRQ ATA status bits must already be known to be clear on both slave and master
; outputs: data stored in EDI; EDI and EBP advanced, EBX decremented
; flags: on success Zero flag set, Carry clear
pio28_read:
    ;add ebp, [esi + dd_stLBA]	; convert relative LBA to absolute LBA

    mov ecx, ebp			; save a working copy
    mov al, bl		; set al= sector count (0 means 256 sectors)
    or dl, 2		; dx = sectorcount port -- usually port 1f2
    out dx, al
    mov al, cl		; ecx currently holds LBA
    inc edx			; port 1f3 -- LBAlow
    out dx, al
    mov al, ch
    inc edx			; port 1f4 -- LBAmid
    out dx, al
    bswap ecx
    mov al, ch		; bits 16 to 23 of LBA
    inc edx			; port 1f5 -- LBAhigh
    out dx, al
    mov al, cl			; bits 24 to 28 of LBA
    ;or al, byte [esi + dd_sbits]	; master/slave flag | 0xe0
    or al, 0xe0
    inc edx				; port 1f6 -- drive select
    out dx, al
 
    inc edx			; port 1f7 -- command/status
    mov al, 0x20		; send "read" command to drive
    out dx, al
 
; ignore the error bit for the first 4 status reads -- ie. implement 400ns delay on ERR only
; wait for BSY clear and DRQ set
    mov ecx, 4
.lp1:
    in al, dx		; grab a status byte
    test al, 0x80		; BSY flag set?
    jne short .retry
    test al, 8		; DRQ set?
    jne short .data_rdy
.retry:
    dec ecx
    jg short .lp1
; need to wait some more -- loop until BSY clears or ERR sets (error exit if ERR sets)
 
.pior_l:
    in al, dx		; grab a status byte
    test al, 0x80		; BSY flag set?
    jne short .pior_l	; (all other flags are meaningless if BSY is set)
    test al, 0x21		; ERR or DF set?
    jne short .fail
.data_rdy:
; if BSY and ERR are clear then DRQ must be set -- go and read the data
    sub dl, 7		; read from data port (ie. 0x1f0)
    mov cx, 256
    rep insw		; gulp one 512b sector into edi
    or dl, 7		; "point" dx back at the status register
    in al, dx		; delay 400ns to allow drive to set new values of BSY and DRQ
    in al, dx
    in al, dx
    in al, dx
 
; After each DRQ data block it is mandatory to either:
; receive and ack the IRQ -- or poll the status port all over again
 
    inc ebp			; increment the current absolute LBA
    dec ebx			; decrement the "sectors to read" count
    test bl, bl		; check if the low byte just turned 0 (more sectors to read?)
    jne short .pior_l
 
    sub dx, 7		; "point" dx back at the base IO port, so it's unchanged
    ;sub ebp, [esi + dd_stLBA]	; convert absolute lba back to relative
; "test" sets the zero flag for a "success" return -- also clears the carry flag
    test al, 0x21		; test the last status ERR bits
    je short .done
.fail:
    stc
    jmp $
.done:
    ret

section .data

message db "Hello MBR"
message.size equ $-message