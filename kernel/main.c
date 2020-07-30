#include <stddef.h>
#include <stdint.h>
#include "arch/x86/x86.h"
#include "osloader/load.h"
#include "libs/log.h"
#include "defs/defs.h"
#include "libs/kustd.h"
#include "buddy.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/interrupt_extern.h"
#include "arch/x86/pic8259a.h"
#include "int_handle.h"
#include "arch/x86/pit8254.h"
#include "mms.h"

mms_t *mem_init(KernelBootArgs *arg) {
    x86_read_cr3();
}

void kernel_bootargs_log(KernelBootArgs *arg) {
    LOG_VAR(arg->mr_max_length_index);
    LOG_VAR(arg->mr_size);
    LOG_VAR(arg->mrs->base);
    LOG_VAR(arg->mrs->length);
    LOG_VAR(arg->next_free_paddr);
    LOG_VAR(arg->next_free_vaddr);
    LOG_VAR(arg->pde_vaddr);
}


void kernel_main() {
    KernelBootArgs arg = *(KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;
    kernel_bootargs_log(&arg);

    x86_int_init_all_desc();
    x86_int_set_common_handle(int_handle);
    x86_int_lidt();
    x86_pic8259a_init(0x20, 0x28);
    x86_pit8254_init(100);
    x86_sti();
    
}