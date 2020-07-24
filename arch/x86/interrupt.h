#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stdint.h>

#define GATE_TYPE_INT   (0xe)
#define GATE_TYPE_TASK  (0x5)
#define GATE_TYPE_TRAP  (0xf)
#define GATE_DPL_0      (0 << 5)
#define GATE_DPL_1      (1 << 5)
#define GATE_DPL_2      (2 << 5)
#define GATE_P          (1 << 7)

typedef struct {
    uint16_t offset_0_15;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset_16_31;
} InterruptDescriptorEntry;

extern InterruptDescriptorEntry x86_idt[256];

extern uint16_t x86_idt_limit;
extern InterruptDescriptorEntry* x86_idt_ptr;

void x86_init_all_desc();
void x86_assign_handler(uint8_t n, uint32_t offset, uint16_t selector);
void x86_lidt();

#endif