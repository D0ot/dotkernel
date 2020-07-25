#ifndef _INTERRUPT_ASM_H_
#define _INTERRUPT_ASM_H_

#include <stdint.h>
#include <stddef.h>
#include "interrupt.h"


extern InterruptDescriptorEntry x86_idt[256];
extern uint16_t x86_idt_limit;
extern InterruptDescriptorEntry* x86_idt_ptr;
extern x86_irq_common_handle_ptr x86_irq_common_handle;
typedef void* x86_irq_handle_sym;

// for example &x86_irq0_handle will be the address
// of handle defined in interrupt.s
// this handle is directly called by x86 processor

extern x86_irq_handle_sym x86_irq0_handle;
extern x86_irq_handle_sym x86_irq1_handle;
extern x86_irq_handle_sym x86_irq2_handle;
extern x86_irq_handle_sym x86_irq3_handle;
extern x86_irq_handle_sym x86_irq4_handle;
extern x86_irq_handle_sym x86_irq5_handle;
extern x86_irq_handle_sym x86_irq6_handle;
extern x86_irq_handle_sym x86_irq7_handle;
extern x86_irq_handle_sym x86_irq8_handle;
extern x86_irq_handle_sym x86_irq9_handle;

extern x86_irq_handle_sym x86_irq10_handle;
extern x86_irq_handle_sym x86_irq11_handle;
extern x86_irq_handle_sym x86_irq12_handle;
extern x86_irq_handle_sym x86_irq13_handle;
extern x86_irq_handle_sym x86_irq14_handle;
extern x86_irq_handle_sym x86_irq15_handle;
extern x86_irq_handle_sym x86_irq16_handle;
extern x86_irq_handle_sym x86_irq17_handle;
extern x86_irq_handle_sym x86_irq18_handle;
extern x86_irq_handle_sym x86_irq19_handle;

extern x86_irq_handle_sym x86_irq20_handle;
extern x86_irq_handle_sym x86_irq21_handle;
extern x86_irq_handle_sym x86_irq22_handle;
extern x86_irq_handle_sym x86_irq23_handle;
extern x86_irq_handle_sym x86_irq24_handle;
extern x86_irq_handle_sym x86_irq25_handle;
extern x86_irq_handle_sym x86_irq26_handle;
extern x86_irq_handle_sym x86_irq27_handle;
extern x86_irq_handle_sym x86_irq28_handle;
extern x86_irq_handle_sym x86_irq29_handle;

extern x86_irq_handle_sym x86_irq30_handle;
extern x86_irq_handle_sym x86_irq31_handle;
extern x86_irq_handle_sym x86_irq32_handle;
extern x86_irq_handle_sym x86_irq33_handle;
extern x86_irq_handle_sym x86_irq34_handle;
extern x86_irq_handle_sym x86_irq35_handle;
extern x86_irq_handle_sym x86_irq36_handle;
extern x86_irq_handle_sym x86_irq37_handle;
extern x86_irq_handle_sym x86_irq38_handle;
extern x86_irq_handle_sym x86_irq39_handle;

extern x86_irq_handle_sym x86_irq40_handle;
extern x86_irq_handle_sym x86_irq41_handle;
extern x86_irq_handle_sym x86_irq42_handle;
extern x86_irq_handle_sym x86_irq43_handle;
extern x86_irq_handle_sym x86_irq44_handle;
extern x86_irq_handle_sym x86_irq45_handle;
extern x86_irq_handle_sym x86_irq46_handle;
extern x86_irq_handle_sym x86_irq47_handle;
extern x86_irq_handle_sym x86_irq48_handle;
extern x86_irq_handle_sym x86_irq49_handle;

extern x86_irq_handle_sym x86_irq50_handle;
extern x86_irq_handle_sym x86_irq51_handle;
extern x86_irq_handle_sym x86_irq52_handle;
extern x86_irq_handle_sym x86_irq53_handle;
extern x86_irq_handle_sym x86_irq54_handle;
extern x86_irq_handle_sym x86_irq55_handle;
extern x86_irq_handle_sym x86_irq56_handle;
extern x86_irq_handle_sym x86_irq57_handle;
extern x86_irq_handle_sym x86_irq58_handle;
extern x86_irq_handle_sym x86_irq59_handle;

extern x86_irq_handle_sym x86_irq60_handle;
extern x86_irq_handle_sym x86_irq61_handle;
extern x86_irq_handle_sym x86_irq62_handle;
extern x86_irq_handle_sym x86_irq63_handle;


extern x86_irq_handle_sym x86_irq255_handle;


#endif