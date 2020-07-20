#include "buddy.h"
#include "libs/common.h"
#include "libs/log.h"



void buddy_init(BuddySystem *bs, void*managed, uint32_t managed_size, void*buddy_system) {
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


void buddy_free(BuddySystem *bs, void *addr) {
    int32_t index = buddy_addr2pageindex(bs, addr);
    buddy_aux_free(bs, index);
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
    
    for(int i = 0; i < 19; ++i) {
        int32_t iter = bs->indices[i];
        LOG_INFO("block page num : %x", POWER_OF_2(i));
        while(iter != -1) {
            LOG_INFO("\t block index %x", iter);
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