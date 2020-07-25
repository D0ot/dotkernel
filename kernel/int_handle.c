#include "int_handle.h"
#include "libs/log.h"

void int_handle(
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
) {
    LOG_INFO("enter interrupt %d", irq_num);
    LOG_INFO("cs:eip = %x:%x, error_code = %x", cs, eip, error_code);
}
