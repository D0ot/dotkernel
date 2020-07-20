#include <stddef.h>
#include <stdint.h>
#include "arch/x86/x86.h"
#include "osloader/load.h"
#include "libs/log.h"
#include "defs/defs.h"
#include "libs/kustd.h"
#include "bitmap.h"
#include "buddy.h"


void kernel_main() {
    KernelBootArgs arg = *(KernelBootArgs*) KERNEL_BOOT_ARGS_ADDR;
    LOG_VAR(arg.next_free_vaddr);
    BuddySystem bs;
    uint8_t buf[10000];
    buddy_init(&bs, (void*)0x80000000, (1 << 22) + (2 << 12), buf);
    buddy_debug_print(&bs);
    void *ret1 = buddy_alloc(&bs, 1);
    LOG_VAR(ret1);
    buddy_debug_print(&bs);
    void *ret4 = buddy_alloc(&bs, 1);
    LOG_VAR(ret4);
    buddy_debug_print(&bs);
    void *ret2 = buddy_alloc(&bs, 2);
    LOG_VAR(ret2);
    buddy_debug_print(&bs);
    void *ret3 = buddy_alloc(&bs, 3);
    LOG_VAR(ret3);
    buddy_free(&bs, ret1);
    buddy_free(&bs, ret2);
    buddy_free(&bs, ret3);
    buddy_free(&bs, ret4);
    buddy_debug_print(&bs);
}