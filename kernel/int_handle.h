#ifndef _INT_HANDLE_H_
#define _INT_HANDLE_H_

#include <stdint.h>
#include "arch/x86/interrupt.h"

void int_handle(
    uint32_t irq_num,

#ifdef X86_IRQ_PUSHA
    uint32_t edi,
    uint32_t esi,
    uint32_t ebp,
    uint32_t orginal_esp,
    uint32_t ebx,
    uint32_t edx,
    uint32_t ecx,
    uint32_t eax,
#endif

    uint32_t error_code,
    uint32_t eip,
    uint32_t cs,
    uint32_t eflags
);




#endif