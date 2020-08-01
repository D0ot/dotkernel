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
#include "libs/paging.h"
#include "defs/defs.h"
#include "buddy.h"
#include "int_handle.h"
#include "arch/x86/pit8254.h"
#include "mms.h"
#include "libs/bochs.h"

mms_t *mem_init(KernelBootArgs *arg) {

    // mov pde to new place
    uint32_t next_free = align_to((uint32_t)arg->next_free_vaddr, sizeof(pde_t) * 1024);
    pde_t *pde_vaddr = (pde_t*)next_free;
    next_free += sizeof(pde_t) * 1024;
    memcpy((void*)pde_vaddr, (void*)(0xffc00000), sizeof(pde_t) * 1024);
    uint32_t pde_index = (uint32_t)pde_vaddr >> 22;
    uint32_t pde_paddr = paging_paddr_in_pdep(pde_vaddr + pde_index) | (~0xffc00000 & (uint32_t)pde_vaddr);

    uint32_t kernel_base_paddr = paging_paddr_in_pdep(pde_vaddr + (KERNEL_BASE >> 22));
    uint32_t pte_paddr = pde_paddr + POWER_OF_2(12);
    pte_t *pte_vaddr = (pte_t*)next_free;
    next_free += sizeof(pte_t) * 1024 * 4;
    pte_t pte_tmp;
    pte_tmp.pte = 0;
    pte_tmp.p = 1;
    pte_tmp.rw = 1;


    // set pte
    for(int i = 0; i < POWER_OF_2(12); ++i) {
        pte_vaddr[i] = pte_tmp;
        paging_set_pte_addr(pte_vaddr + i, kernel_base_paddr + (i << 12));
    }
    
    // set pde
    for(int i = 0; i < 4; ++i) {
        pde_t *tmp = pde_vaddr + (KERNEL_BASE >> 22) + i;
        tmp->ps = 0;
        paging_set_pde_table_addr(tmp, pte_paddr + (i << 12));
    }

    // if we want to access physical page at A_paddr
    // we can set addr in PDE to vaddr of stub_page
    // the stub_page is an PTE, using vaddr "stbu_page" can
    // access the PTE, then we can map another vaddr(assuming it is B_vaddr) to A_paddr
    // accessing B_vaddr is accessing physical page at A_paddr
    uint32_t stub_page = next_free;
    next_free += POWER_OF_2(12);

    pde_vaddr[0x3ff].ps = 0;
    paging_set_pde_table_addr(pde_vaddr + 0x3ff, pte_paddr + (4 << 12));

    for(int i = 0; i < 1024; ++i) {
        pte_vaddr[i + POWER_OF_2(12) + i].pte = 0;
    }


    // set new cr3
    x86_write_cr3(pde_paddr | (1 << 3) | (1 << 4));

    uint32_t res_phy = (arg->mrs[arg->mr_max_length_index].length) 
        - ( (uint32_t)arg->next_free_paddr - (arg->mrs[arg->mr_max_length_index].base));

    // initialize the buddy system for physical address
    next_free = align_to(next_free, sizeof(BuddySystem));
    BuddySystem *pbs = (BuddySystem*)(next_free);
    next_free += sizeof(BuddySystem);
    buddy_init(pbs, arg->next_free_paddr, res_phy, (void*)next_free);
    next_free += buddy_get_memreq(res_phy);
    

    // initialize the buddy system for virtual address used for kernel(from 0x8000_0000 to 0xffff_ffff)
    // 16MiB for kernel allocated
    // the last 4MiB is used for access PTE and PDE, so minus (4+1) * 4MiB
    uint32_t res_kernel = (0x80000000 - ((4 << 20) * KERNEL_INITIAL_4MPAGE_NUM)) - (4 << 20);
    BuddySystem *kbs = (BuddySystem*)(next_free);    
    next_free += sizeof(BuddySystem);
    buddy_init(kbs, (void*)(KERNEL_BASE + (4 << 20) * KERNEL_INITIAL_4MPAGE_NUM), res_kernel, (void*)next_free);
    next_free += buddy_get_memreq(res_kernel);

    mms_t *mms = (mms_t*)next_free;
    next_free += sizeof(mms_t);

    mms_init(mms, pde_vaddr, pbs, kbs, (pte_t*)stub_page);

    arg->next_free_vaddr = (void*)next_free;
    return mms;
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
    mms_t *mms = mem_init(&arg);

    x86_int_init_all_desc();
    x86_int_set_common_handle(int_handle);
    x86_int_lidt();
    x86_pic8259a_init(0x20, 0x28);
    x86_pit8254_init(100);
    //x86_sti();
    
}