#include "pic8259a.h"
#include "x86.h"

// int_num_base must be divisible by 8
void x86_pic8259a_init(uint8_t int_num_base1, uint8_t int_num_base2) {

    x86_outb(PIC_8259A_1_COMMAND, 0x11);
    x86_outb(PIC_8259A_2_COMMAND, 0x11);

    x86_outb(PIC_8259A_1_DATA, int_num_base1);
    x86_outb(PIC_8259A_2_DATA, int_num_base2);

    x86_outb(PIC_8259A_2_DATA, 0x04);
    x86_outb(PIC_8259A_1_DATA, 0x04);

    x86_outb(PIC_8259A_1_DATA, 0x01);
    x86_outb(PIC_8259A_2_DATA, 0x01);

    x86_pic8259a_mask(0xfe, 0xff);
}

void x86_pic8259a_mask(uint8_t irq_mask1, uint8_t irq_mask2) {
    x86_outb(PIC_8259A_1_DATA, irq_mask1);
    x86_outb(PIC_8259A_2_DATA, irq_mask2);
}

void x86_pic8259a_eoi(uint8_t irq) {
    if(irq >= 8) {
        x86_outb(PIC_8259A_2_COMMAND, PIC_8259A_EOI);
    } else {
        x86_outb(PIC_8259A_1_COMMAND, PIC_8259A_EOI);
    }
}