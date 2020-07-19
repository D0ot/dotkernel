#include <stddef.h>
#include <stdint.h>
#include "arch/x86/x86.h"
#include "osloader/load.h"
#include "libs/log.h"
#include "defs/defs.h"
#include "libs/common.h"
#include "bitmap.h"


void kernel_main() {
    KernelBootArgs arg = *(KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;
    LOG_VAR(arg.next_free_vaddr);
    
    uint8_t buf[1024];
    BitMap bm;
    bm.bitmap = buf;
    bm.managed = arg.next_free_vaddr;
    bm.managed_size = 1024 * 8;
    bm.ratio = 2;

    bitmap_init(&bm);
    void* ret1 = bitmap_alloc(&bm, 10);
    LOG_VAR(ret1);
    void* ret2 = bitmap_alloc(&bm, 3);
    LOG_VAR(ret2);
    void* ret3 = bitmap_alloc(&bm, 5);
    LOG_VAR(ret3);
}