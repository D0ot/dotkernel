#include <stddef.h>
#include <stdint.h>
#include "arch/x86/x86.h"
#include "osloader/load.h"
#include "libs/log.h"
#include "defs/defs.h"

void kernel_main() {
    KernelBootArgs *arg = (KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;
    LOG_INFO("arg->phy_pdes : %x", arg->phy_pdes);
}