#include <stddef.h>
#include <stdint.h>
#include "osloader/load.h"
#include "arch/x86/x86.h"
#include "arch/x86/interrupt.h"
#include "arch/x86/interrupt_extern.h"
#include "arch/x86/pic8259a.h"
#include "libs/log.h"
#include "libs/kustd.h"
#include "libs/utils.h"
#include "defs/defs.h"
#include "buddy.h"
#include "int_handle.h"
#include "arch/x86/pit8254.h"
#include "mms.h"

mms_t *mem_init(KernelBootArgs *arg) {
    uint32_t res_phy = (arg->mrs[arg->mr_max_length_index].length) 
        - ( (uint32_t)arg->next_free_paddr - (arg->mrs[arg->mr_max_length_index].base));
    uint32_t mem_req = buddy_get_memreq(res_phy);


    // initialize the buddy system for physical address
    uint32_t next_free = align_to((uint32_t)arg->next_free_vaddr, sizeof(BuddySystem));
    BuddySystem *pbs = (BuddySystem*)(next_free);
    next_free += sizeof(BuddySystem);
    buddy_init(pbs, arg->next_free_paddr, res_phy, (void*)next_free);
    next_free += mem_req;

    // initialize the buddy system for virtual address used for kernel(from 0x8000_0000 to 0xffff_ffff)
    // 16MiB for kernel allocated. 4MiB for PTE and PDE allocated and
    // the last 4MiB is used for access PTE and PDE, so minus 6 * 4MiB
    uint32_t res_kernel = (0x80000000 - ((4 << 20) * 6));
    BuddySystem *kbs = (BuddySystem*)(next_free);
    next_free += sizeof(BuddySystem);
    buddy_init(kbs, (void*)(0x80000000 + (4 << 20) * 5), res_kernel, next_free);
    next_free += buddy_get_memreq(res_kernel);
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
    // now the low memroy(address from 0 to 4M) can be freely used


    kernel_bootargs_log(&arg);

    x86_int_init_all_desc();
    x86_int_set_common_handle(int_handle);
    x86_int_lidt();
    x86_pic8259a_init(0x20, 0x28);
    x86_pit8254_init(100);
    x86_sti();
    
}