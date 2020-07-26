#ifndef _PIC8259A_H_
#define _PIC8259A_H_

#include <stdint.h>

#define PIC_8259A_1             0x20
#define PIC_8259A_2             0xA0
#define PIC_8259A_1_COMMAND     PIC_8259A_1
#define PIC_8259A_1_DATA        (PIC_8259A_1 + 1)
#define PIC_8259A_2_COMMAND     PIC_8259A_2
#define PIC_8259A_2_DATA        (PIC_8259A_2 + 1)

#define PIC_8259A_EOI           0x20


void x86_pic8259a_init(uint8_t int_num_base1, uint8_t int_num_base2);
void x86_pic8259a_mask(uint8_t irq_mask1, uint8_t irq_mask2);
void x86_pic8259a_eoi(uint8_t irq);


#endif