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

typedef void(*x86_irq_common_handle_ptr)(
    uint32_t irq_num,

    uint32_t edi,
    uint32_t esi,
    uint32_t ebp,
    uint32_t orginal_esp,
    uint32_t ebx,
    uint32_t edx,
    uint32_t ecx,
    uint32_t eax,

    uint32_t error_code,
    uint32_t eip,
    uint32_t cs,
    uint32_t eflags
);




void x86_int_init_all_desc();
void x86_int_assign_handler(uint8_t n, uint32_t offset, uint16_t selector);
void x86_int_set_common_handle(x86_irq_common_handle_ptr common_handle);
void x86_int_lidt();


#endif