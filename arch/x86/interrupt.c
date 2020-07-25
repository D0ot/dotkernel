#include "interrupt.h"
#include "libs/kustd.h"
#include "interrupt_extern.h"


#define INSTALL_ISR(X) \
    x86_int_assign_handler(X, (uint32_t)&x86_irq##X##_handle, 8)

void x86_int_init_all_desc() {
    memset((void*)x86_idt, 0, sizeof(x86_idt));

    INSTALL_ISR(0);
    INSTALL_ISR(1);
    INSTALL_ISR(2);
    INSTALL_ISR(3);
    INSTALL_ISR(4);
    INSTALL_ISR(5);
    INSTALL_ISR(6);
    INSTALL_ISR(7);
    INSTALL_ISR(8);
    INSTALL_ISR(9);



    INSTALL_ISR(10);
    INSTALL_ISR(11);
    INSTALL_ISR(12);
    INSTALL_ISR(13);
    INSTALL_ISR(14);
    INSTALL_ISR(15);
    INSTALL_ISR(16);
    INSTALL_ISR(17);
    INSTALL_ISR(18);
    INSTALL_ISR(19);



    INSTALL_ISR(20);
    INSTALL_ISR(21);
    INSTALL_ISR(22);
    INSTALL_ISR(23);
    INSTALL_ISR(24);
    INSTALL_ISR(25);
    INSTALL_ISR(26);
    INSTALL_ISR(27);
    INSTALL_ISR(28);
    INSTALL_ISR(29);



    INSTALL_ISR(30);
    INSTALL_ISR(31);
    INSTALL_ISR(32);
    INSTALL_ISR(33);
    INSTALL_ISR(34);
    INSTALL_ISR(35);
    INSTALL_ISR(36);
    INSTALL_ISR(37);
    INSTALL_ISR(38);
    INSTALL_ISR(39);



    INSTALL_ISR(40);
    INSTALL_ISR(41);
    INSTALL_ISR(42);
    INSTALL_ISR(43);
    INSTALL_ISR(44);
    INSTALL_ISR(45);
    INSTALL_ISR(46);
    INSTALL_ISR(47);
    INSTALL_ISR(48);
    INSTALL_ISR(49);



    INSTALL_ISR(50);
    INSTALL_ISR(51);
    INSTALL_ISR(52);
    INSTALL_ISR(53);
    INSTALL_ISR(54);
    INSTALL_ISR(55);
    INSTALL_ISR(56);
    INSTALL_ISR(57);
    INSTALL_ISR(58);
    INSTALL_ISR(59);

    INSTALL_ISR(60);
    INSTALL_ISR(61);
    INSTALL_ISR(62);
    INSTALL_ISR(63);

    for(int i = 64; i <= 0xff; ++i) {
        x86_int_assign_handler(i, (uint32_t)&x86_irq255_handle, 0x08);
    }
}

void x86_int_assign_handler(uint8_t n, uint32_t offset, uint16_t selector) {
    InterruptDescriptorEntry* ide = x86_idt + n;
    ide->offset_0_15 = (uint16_t) ((offset) & 0xffff);
    ide->offset_16_31 = (uint16_t) ((offset) >> 16);
    ide->selector = selector;
    ide->zero = 0;
    ide->type = GATE_TYPE_INT | GATE_P | GATE_DPL_0;
}


void x86_int_set_common_handle(x86_irq_common_handle_ptr common_handle) {
    x86_irq_common_handle = common_handle;
}

void x86_int_lidt() {
    __asm__ (
        "lidt [%[ptr]]\n\t"
        :
        : [ptr]"Q"(&x86_idt_limit)
    );
}