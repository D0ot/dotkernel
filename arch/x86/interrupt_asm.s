INT_DESC_SIZE   equ         64
section .data

global x86_idt
global x86_idt_limit
global x86_idt_ptr
global x86_irq_common_handle

align                                   INT_DESC_SIZE
x86_idt times 0x100         dq          0
x86_idt_limit               dw          x86_idt_limit - x86_idt - 1
x86_idt_ptr                 dd          x86_idt
align                                   4
x86_irq_common_handle       dd          0


%define NO_ERROR_CODE       push 0
%define HAS_ERROR_CODE      nop


section .text
; first is irq number, second is push 0 or not(when has error_code in stack)
%macro X86_IRQ_HANDLE 2
global x86_irq%1_handle
x86_irq%1_handle:
    %2
    pushad
    push %1
    call [x86_irq_common_handle]
    add esp, 4
    popad
    add esp, 4
    iret
%endmacro 

X86_IRQ_HANDLE 0,   NO_ERROR_CODE
X86_IRQ_HANDLE 1,   NO_ERROR_CODE
X86_IRQ_HANDLE 2,   NO_ERROR_CODE
X86_IRQ_HANDLE 3,   NO_ERROR_CODE
X86_IRQ_HANDLE 4,   NO_ERROR_CODE
X86_IRQ_HANDLE 5,   NO_ERROR_CODE   
X86_IRQ_HANDLE 6,   NO_ERROR_CODE 
X86_IRQ_HANDLE 7,   NO_ERROR_CODE
X86_IRQ_HANDLE 8,   HAS_ERROR_CODE
X86_IRQ_HANDLE 9,   NO_ERROR_CODE

X86_IRQ_HANDLE 10,  HAS_ERROR_CODE
X86_IRQ_HANDLE 11,  HAS_ERROR_CODE
X86_IRQ_HANDLE 12,  HAS_ERROR_CODE
X86_IRQ_HANDLE 13,  HAS_ERROR_CODE
X86_IRQ_HANDLE 14,  HAS_ERROR_CODE
X86_IRQ_HANDLE 15,  NO_ERROR_CODE
X86_IRQ_HANDLE 16,  NO_ERROR_CODE
X86_IRQ_HANDLE 17,  HAS_ERROR_CODE
X86_IRQ_HANDLE 18,  NO_ERROR_CODE
X86_IRQ_HANDLE 19,  NO_ERROR_CODE

X86_IRQ_HANDLE 20,  NO_ERROR_CODE
X86_IRQ_HANDLE 21,  NO_ERROR_CODE
X86_IRQ_HANDLE 22,  NO_ERROR_CODE
X86_IRQ_HANDLE 23,  NO_ERROR_CODE
X86_IRQ_HANDLE 24,  NO_ERROR_CODE
X86_IRQ_HANDLE 25,  NO_ERROR_CODE
X86_IRQ_HANDLE 26,  NO_ERROR_CODE
X86_IRQ_HANDLE 27,  NO_ERROR_CODE
X86_IRQ_HANDLE 28,  NO_ERROR_CODE
X86_IRQ_HANDLE 29,  NO_ERROR_CODE

X86_IRQ_HANDLE 30,  HAS_ERROR_CODE
X86_IRQ_HANDLE 31,  NO_ERROR_CODE

; for user define.
X86_IRQ_HANDLE 32,  NO_ERROR_CODE
X86_IRQ_HANDLE 33,  NO_ERROR_CODE
X86_IRQ_HANDLE 34,  NO_ERROR_CODE
X86_IRQ_HANDLE 35,  NO_ERROR_CODE
X86_IRQ_HANDLE 36,  NO_ERROR_CODE
X86_IRQ_HANDLE 37,  NO_ERROR_CODE
X86_IRQ_HANDLE 38,  NO_ERROR_CODE
X86_IRQ_HANDLE 39,  NO_ERROR_CODE
X86_IRQ_HANDLE 40,  NO_ERROR_CODE
X86_IRQ_HANDLE 41,  NO_ERROR_CODE
X86_IRQ_HANDLE 42,  NO_ERROR_CODE
X86_IRQ_HANDLE 43,  NO_ERROR_CODE
X86_IRQ_HANDLE 44,  NO_ERROR_CODE
X86_IRQ_HANDLE 45,  NO_ERROR_CODE
X86_IRQ_HANDLE 46,  NO_ERROR_CODE
X86_IRQ_HANDLE 47,  NO_ERROR_CODE
X86_IRQ_HANDLE 48,  NO_ERROR_CODE
X86_IRQ_HANDLE 49,  NO_ERROR_CODE
X86_IRQ_HANDLE 50,  NO_ERROR_CODE
X86_IRQ_HANDLE 51,  NO_ERROR_CODE
X86_IRQ_HANDLE 52,  NO_ERROR_CODE
X86_IRQ_HANDLE 53,  NO_ERROR_CODE
X86_IRQ_HANDLE 54,  NO_ERROR_CODE
X86_IRQ_HANDLE 55,  NO_ERROR_CODE
X86_IRQ_HANDLE 56,  NO_ERROR_CODE
X86_IRQ_HANDLE 57,  NO_ERROR_CODE
X86_IRQ_HANDLE 58,  NO_ERROR_CODE
X86_IRQ_HANDLE 59,  NO_ERROR_CODE

X86_IRQ_HANDLE 60,  NO_ERROR_CODE
X86_IRQ_HANDLE 61,  NO_ERROR_CODE
X86_IRQ_HANDLE 62,  NO_ERROR_CODE
X86_IRQ_HANDLE 63,  NO_ERROR_CODE


X86_IRQ_HANDLE 255, NO_ERROR_CODE
