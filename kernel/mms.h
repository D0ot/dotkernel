#ifndef _MMS_H_
#define _MMS_H_

#include <stdint.h>
#include <stddef.h>
#include "buddy.h"
#include "libs/paging.h"

typedef struct 
{
    pde_t *pde;
    BuddySystem *pbs;
    BuddySystem *vbs;
} mms_t;


// interface functions
void mms_init(mms_t *mms, pde_t* pde, BuddySystem *pbs);
void *mms_kalloc(mms_t *mms, uint32_t page_num);
void *mms_alloc(mms_t *mms, BuddySystem *vbs, uint32_t page_num);
void *mms_alloc_phy(mms_t *mms, uint32_t page_num);
void mms_free(mms_t *mms, BuddySystem *vbs, void *vaddr);


// being used in functions below 
void mms_map(mms_t *mms, void *vaddr, void *paddr, uint32_t page_num);
void mms_unmap(mms_t *mms, void *vaddr, uint32_t page_num);


// aux functions
void *mms_vir2phy(mms_t *mms, void *vaddr);



#endif