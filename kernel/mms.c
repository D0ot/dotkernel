#include "mms.h"
#include "arch/x86/x86.h"

void mms_init(mms_t *mms, pde_t *pde, BuddySystem *pbs, BuddySystem *kbs, pte_t *pte_stub) {
    mms->pbs = pbs;
    mms->pde = pde;
    mms->kbs = kbs;
    mms->pte_stub = pte_stub;
}


void *mms_kalloc(mms_t *mms, uint32_t page_num) {
    return mms_alloc(mms, mms->kbs, page_num);
}


void *mms_alloc(mms_t *mms, BuddySystem *vbs, uint32_t page_num) {
    void *paddr = mms_alloc_phy(mms, page_num);
    if(paddr == NULL) {
        return NULL;
    }
    void *vaddr = buddy_alloc(vbs, page_num);
    if(vaddr == NULL) {
        return NULL;
    }
    for(uint32_t i = 0; i < page_num; ++i) {
        mms_map(mms, (void*)((uint32_t)vaddr + (i << 12)), (void*)((uint32_t)paddr + (i << 12)));
    }
    return vaddr;
}


void *mms_alloc_phy(mms_t *mms, uint32_t page_num) {
    return buddy_alloc(mms->pbs, page_num);
}

void mms_free(mms_t *mms, BuddySystem *vbs, void *vaddr) {
    void *paddr = mms_vir2phy(mms, vaddr);
    uint32_t page_num = buddy_free(mms->pbs, paddr);
    buddy_free(vbs, vaddr);
    for(uint32_t i = 0; i < page_num; ++i) {
        mms_unmap(mms, (uint32_t)vaddr + (i << 12));
    }
}

void mms_kfree(mms_t *mms, void *vaddr) {
    mms_free(mms, mms->kbs, vaddr);
}

void mms_map(mms_t *mms, void *vaddr , void *paddr) {
    pde_t *pde = mms->pde + ((uint32_t)vaddr >> 22);
    pte_t *pte_paddr = NULL;
    if(pde->p) {
        pte_paddr = paging_paddr_in_pdet(pde);
    }else {
        pde->pde = 0;
        pte_paddr = mms_alloc_phy(mms, 1);
        paging_set_pde_table_addr(pde, pte_paddr);
        pde->p = 1;
        pde->rw = 1;
    }

    mms->pte_stub[0].rw = 1;
    mms->pte_stub[0].p = 1;
    paging_set_pte_addr(mms->pte_stub, pte_paddr);

    pte_t *pte_vaddr = (pte_t*)(0xffffffff - ((4 << 20) - 1));
    x86_invlpg((void*)pte_vaddr);
    pte_t *pte = pte_vaddr + (((uint32_t)vaddr & 0x003ff000) >> 12);
    pte->pte = 0;
    pte->p = 1;
    pte->rw = 1;
    paging_set_pte_addr(pte, (uint32_t)paddr);

    mms->pte_stub[0].p = 0;
}

void mms_unmap(mms_t *mms, void *vaddr) {
    pde_t *pde = mms->pde + ((uint32_t)vaddr >> 22);
    pte_t *pte_paddr = (pte_t*)paging_paddr_in_pdet(pde);

    mms->pte_stub[0].rw = 1;
    mms->pte_stub[0].p = 1;
    paging_set_pte_addr(mms->pte_stub, pte_paddr);
    
    pte_t *pte_vaddr = (pte_t*)(0xffffffff - ((4 << 20) - 1));

    pte_vaddr[(0x003ff000 & (uint32_t)vaddr) >> 12].p = 0;

    mms->pte_stub[0].p = 0;
}

void *mms_vir2phy(mms_t *mms, void *vaddr) {
    pde_t *pde = mms->pde + ((uint32_t)vaddr >> 22);
    pte_t *pte_paddr = (pte_t*)paging_paddr_in_pdet(pde);

    mms->pte_stub[0].rw = 1;
    mms->pte_stub[0].p = 1;
    paging_set_pte_addr(mms->pte_stub, pte_paddr);
    
    pte_t *pte_vaddr = (pte_t*)(0xffffffff - ((4 << 20) - 1));
    
    uint32_t paddr = paging_paddr_in_pte(pte_vaddr + ((0x003ff000 & (uint32_t)vaddr) >> 12));

    mms->pte_stub[0].p = 0;

    return paddr;
}