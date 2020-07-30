#include "mms.h"

void mms_init(mms_t *mms, pde_t* pde, BuddySystem *pbs) {
    mms->phy = pbs;
    mms->pde = pde;
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
    mms_map(mms, vaddr, paddr, page_num);
    return vaddr;
}


void *mms_alloc_phy(mms_t *mms, uint32_t page_num) {
    return buddy_alloc(mms->phy, page_num);
}

void mms_free(mms_t *mms, BuddySystem *vbs, void *vaddr) {
    void *paddr = mms_vir2phy(mms, vaddr);
    uint32_t page_num = buddy_free(mms->phy, paddr);
    buddy_free(vbs, vaddr);
    mms_unmap(mms, vaddr, page_num);
}


void mms_map(mms_t *mms, void *vaddr , void *paddr, uint32_t page_num) {
}

void mms_unmap(mms_t *mms, void *vaddr , uint32_t page_num) {

}

void *mms_vir2phy(mms_t *mms, void *vaddr) {
    pde_t *pde = mms->pde + ((uint32_t)vaddr >> 22);
    
}