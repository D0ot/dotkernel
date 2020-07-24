#include <stddef.h>
#include <stdint.h>
#include "arch/x86/x86.h"
#include "osloader/load.h"
#include "libs/log.h"
#include "defs/defs.h"
#include "libs/kustd.h"
#include "buddy.h"
#include "arch/x86/interrupt.h"

void kernel_main() {
    KernelBootArgs arg = *(KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;

    LOG_INFO("%x", x86_idt);
    LOG_INFO("%x", (uint32_t)x86_idt_limit);
    LOG_INFO("%x", x86_idt_ptr);
}