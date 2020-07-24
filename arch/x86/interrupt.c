#include "interrupt.h"
#include "kustd.h"

InterruptDescriptorEntry idt[256];

void x86_init_all_desc() {
    memset((void*)idt, 0, sizeof(idt));
}

void x86_assign_handler(uint8_t n, uint32_t offset, uint16_t selector) {
    InterruptDescriptorEntry* ide = idt + n;
    ide->offset_0_15 = (uint16_t) ((offset) & 0xffff);
    ide->offset_16_31 = (uint16_t) ((offset) >> 16);
    ide->selector = selector;
    ide->zero = 0;
    ide->type = GATE_TYPE_INT | GATE_P | GATE_DPL_0;
}

void x86_lidt() {
    __asm__ (
        "nop"
    );
}