%include "defs/defs.s"

extern kernel_main

global kernel_start

NEW_STACK_BASE  equ     KERNEL_BASE + (KERNEL_INITIAL_4MPAGE_NUM << 22)

kernel_start:
    mov esp, NEW_STACK_BASE
    mov ebp, NEW_STACK_BASE
    call kernel_main
.inf_loop:
    jmp .inf_loop