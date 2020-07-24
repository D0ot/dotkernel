INT_DESC_SIZE   equ         64
section .data

global x86_idt
global x86_idt_limit
global x86_idt_ptr

align                                   INT_DESC_SIZE
x86_idt times 0x100         dq          0
x86_idt_limit               dw          x86_idt_limit - x86_idt - 1
x86_idt_ptr                 dd          x86_idt
align                                   4
x86_irq_sub_handle_ptr times 0x100  dd          0




