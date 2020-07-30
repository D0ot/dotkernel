#include "buddy.h"
#include "libs/kustd.h"
#include "libs/utils.h"
#include "libs/log.h"

/**
 * \fn void buddy_init(BuddySystem *bs, void*managed, uint32_t managed_size, void*buddy_system);
 * \brief initialize the BuddySystem data structure.
 * \param bs buddy system internal data.
 * \param managed pointer to memory we want to manage.
 * \param managed_size the size of memory we want managed.
 * \param buddy_system the memory that buddy system itself usese.
 */

void buddy_init(BuddySystem *bs, void *managed, uint32_t managed_size, void *buddy_system) {
    bs->managed = managed;
    bs->managed_size = managed_size;
    bs->buddy_system = buddy_system;
    bs->managed_page_num = managed_size / PAGE_SIZE;
    bs->buddy_system_size = bs->managed_page_num * sizeof(BuddyList);
    bs->buddy_system = buddy_system;

    for(uint32_t i = 0; i < sizeof(bs->indices) / sizeof(bs->indices[0]); ++i) {
        bs->indices[i] = -1;
    }

    uint32_t page_cnt = 0;
    for(int i = 19; i >= 0; --i) {
        if(bs->managed_page_num & (1 << i)) {
            bs->indices[i] = page_cnt;
            bs->buddy_system[page_cnt].next = -1;
            bs->buddy_system[page_cnt].pow = i;
            page_cnt += POWER_OF_2(i);
        }
    }
}

/**
 * \fn void *buddy_alloc(BuddySystem *bs, uint32_t page_num)
 * \brief alloc a block of memory, size is "page_num"(in page).
 * \param bs buddy system internal data.
 * \param page_num the number of page we want to alloc
 *  a wise choice is that the "page_num" is a power of 2,
 *  otherwise the memory will be wasted.
 */

void *buddy_alloc(BuddySystem *bs, uint32_t page_num) {
    uint8_t pow = buddy_aux_fit(page_num);
    LOG_VAR(pow);
    int32_t ret = buddy_aux_alloc(bs, pow);
    LOG_VAR(ret);
    if(ret == -1) {
        return NULL;
    }
    return buddy_pageindex2addr(bs, ret);
}

/**
 * \fn void buddy_free(BuddySystem *bs, void *addr)
 * \brief free a block of memory.
 * \param bs buddy system internal data.
 * \param addr the address of the memory block to be freed.
 * \return the page num of the block to be freed
 */

uint32_t buddy_free(BuddySystem *bs, void *addr) {
    int32_t index = buddy_addr2pageindex(bs, addr);
    uint32_t ret = POWER_OF_2(bs->buddy_system[index].pow);
    buddy_aux_free(bs, index);
    return ret;
}

/**
 * \fn uint32_t buddy_get_memreq(uint32_t managed_size)
 * \brief get the memory needed for buddy system, 
 * \param managed_size the size of managed memory
 */

uint32_t buddy_get_memreq(uint32_t managed_size) {
    return managed_size / PAGE_SIZE * sizeof(BuddyList);
}



int32_t buddy_aux_alloc(BuddySystem *bs, uint8_t pow) {
    if(pow == 20) {
        return -1;
    }
    if(bs->indices[pow] != -1) {
        int32_t ret = bs->indices[pow];
        bs->indices[pow] = bs->buddy_system[ret].next;
        return ret;
    } else {
        int32_t half1 = buddy_aux_alloc(bs, pow + 1);
        if(half1 == -1) {
            return -1;
        }
        int32_t half2 = half1 + POWER_OF_2(bs->buddy_system[half1].pow - 1);
        bs->buddy_system[half1].pow = pow;
        bs->buddy_system[half2].next = -1;
        bs->buddy_system[half2].pow = pow;
        bs->indices[pow] = half2;
        return half1;
    }
}

void buddy_aux_free(BuddySystem *bs, int32_t page_index) {
    uint8_t pow = bs->buddy_system[page_index].pow;
    LOG_INFO("pow : %x", (uint32_t)pow);
    int32_t last = -1;
    int32_t i = bs->indices[pow];
    if(i == -1) { 
        // in the list(denoted by bs->indices[pow]), no items
        // so we just add it to the list
        bs->indices[pow] = page_index;
        bs->buddy_system[page_index].next = -1;
    } else if(page_index < i){
        if(page_index + POWER_OF_2(pow) == i) {
            // merge the page
            bs->buddy_system[page_index].pow = pow + 1;
            bs->indices[pow] = bs->buddy_system[i].next;
            buddy_aux_free(bs, page_index);
        } else {
            // just add it, no merge
            bs->buddy_system[page_index].next = i;
            bs->indices[pow] = page_index;
        }
    } else {
        // i < page_index, iteration is needed

        while(bs->buddy_system[i].next != -1 && bs->buddy_system[i].next <= page_index) {
            last = i;
            i = bs->buddy_system[i].next;
        }

        if(bs->buddy_system[i].next == -1) {
            if(buddy_aux_mergable(i, page_index, pow)) {
                bs->buddy_system[i].pow = pow + 1;
                if(last == -1) {
                    bs->indices[pow] = -1;
                } else {
                    bs->buddy_system[last].next = -1;
                }
                buddy_aux_free(bs, i);
                
            } else {
                bs->buddy_system[i].next = page_index;
                bs->buddy_system[page_index].next = -1;
            }
        } else {
            if(buddy_aux_mergable(i, page_index, pow)) {
                bs->buddy_system[i].pow = pow + 1;
                bs->buddy_system[last].next = bs->buddy_system[i].next;

                buddy_aux_free(bs, i);
            } else if(buddy_aux_mergable(page_index, bs->buddy_system[i].next, pow)) {
                bs->buddy_system[page_index].pow = pow + 1;
                bs->buddy_system[i].next = bs->buddy_system[(bs->buddy_system[i].next)].next;

                buddy_aux_free(bs, page_index);
            } else {
                int32_t tmp = bs->buddy_system[i].next;
                bs->buddy_system[i].next = page_index;
                bs->buddy_system[page_index].next = tmp;
            }
        }
    }
}


uint8_t buddy_aux_fit(uint32_t page_num) {
    uint8_t pow = 0;
    for(int i = 19; i >= 0; --i) {
        if(page_num & (1 << i)) {
            pow = i;
            break;
        }
    }
    if(page_num - POWER_OF_2(pow)) {
        ++pow;
    }
    return pow;
}


uint8_t buddy_aux_mergable(int32_t page_index1, int32_t page_index2, uint8_t pow) {
    return (page_index1 + POWER_OF_2(pow) == page_index2) && (page_index1 % POWER_OF_2(pow + 1) == 0);
}

void buddy_debug_print(BuddySystem *bs) {
    
    LOG_INFO("buddy_debug_print :");
    for(int i = 0; i < 19; ++i) {
        int32_t iter = bs->indices[i];
        if(iter != -1) {
            LOG_INFO("block size (in pages) : 2^%x", i);
        }
        while(iter != -1) {
            LOG_INFO("\tindex %x", iter);
            iter = bs->buddy_system[iter].next;
        }
    }
}

uint32_t buddy_addr2pageindex(BuddySystem *bs, void *addr) {
    return (addr - bs->managed) / PAGE_SIZE;
}

void *buddy_pageindex2addr(BuddySystem *bs, uint32_t page_index) {
    return bs->managed + page_index * PAGE_SIZE;
}