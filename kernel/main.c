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

void kernel_main() {
    KernelBootArgs arg = *(KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;

    x86_int_init_all_desc();
    x86_int_set_common_handle(int_handle);
    x86_int_lidt();
    x86_pic8259a_init(0x20, 0x28);
    x86_pit8254_init(100);
    x86_sti();
    
}