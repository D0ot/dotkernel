#include <stddef.h>
#include <stdint.h>
#include "arch/x86/x86.h"
#include "osloader/load.h"
#include "libs/log.h"
#include "defs/defs.h"
#include "libs/kustd.h"
#include "buddy.h"

void kernel_main() {
    KernelBootArgs arg = *(KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;
    
}