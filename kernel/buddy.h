#ifndef _BUDDY_H_
#define _BUDDY_H_

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE (POWER_OF_2(12))

typedef struct {
    uint8_t pow; // POWER_OF_2(pow) is the page num of this block.
    int32_t next;
} BuddyList;

typedef struct {
    void *managed;
    uint32_t managed_size;
    uint32_t managed_page_num;
    BuddyList *buddy_system;
    uint32_t buddy_system_size;
    int32_t indices[20];
} BuddySystem;

/**
 * 4k -> 0
 * 4 x 2^1 -> 1
 * 4 x 2^10 -> 10, 4 x 2^10 = 4M
 * 4 x 2^20 -> 20, 4 x 2^20 = 4G
 */



void buddy_init(BuddySystem *bs, void*managed, uint32_t managed_size, void*buddy_system);
void *buddy_alloc(BuddySystem *bs, uint32_t page_num);
uint32_t buddy_free(BuddySystem *bs, void *addr);
uint32_t buddy_get_memreq(uint32_t managed_size);


int32_t buddy_aux_alloc(BuddySystem *bs, uint8_t pow);
void buddy_aux_free(BuddySystem *bs, int32_t page_index);
uint8_t buddy_aux_fit(uint32_t page_num);
uint8_t buddy_aux_mergable(int32_t page_index1, int32_t page_index2, uint8_t pow);
void buddy_debug_print(BuddySystem *bs);

uint32_t buddy_addr2pageindex(BuddySystem *bs, void *addr);
void *buddy_pageindex2addr(BuddySystem *bs, uint32_t page_cnt);



#endif