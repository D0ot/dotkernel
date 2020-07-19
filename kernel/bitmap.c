#include "bitmap.h"
#include "libs/common.h"
#include "libs/log.h"





/**
 * \fn void bitmap_init(BitMap *bm, void*managed, void*bitmap, uint32_t managed_size, uint32_t ratio);
 * \brief Init the data structre to store then needed data
 * \param bm bitmap data struture, to save the whole state.
 * \param managed points to memory we want to manage.
 * \param bitmap points to memory the bitmap itself usese.
 * \param ratio one bit can present ratio * 8 byte.
 */

void bitmap_init(BitMap *bm, void*managed, void*bitmap, uint32_t managed_size, uint32_t ratio) {

    bm->managed = managed;
    bm->managed_size = managed_size;
    bm->bitmap = bitmap;
    bm->ratio = ratio;

    uint32_t bitmap_size = bm->managed_size / bm->ratio / 8;
    bm->bitmap_size = bitmap_size;
    memset(bm->bitmap, 0, bitmap_size);
}

/**
 * \fn void* bitmap_alloc(BitMap *bm, uint32_t size) {
 * \brief allocate a piece of memory.
 * \param pm struct BitMap pointer.
 * \param size size in byte
 * \return the address of memroy allocated, 0 indicats no enough memory
 * Allocate a piece of memroy, the real size(in byte) it occupies is "size" + 4 byte
 * the additional 4 byte is to store the size(in bit). 
 * if the address we get is ADDR, the size(in bit) will be "*(uint32_t*)(ADDR)"
 */
void* bitmap_alloc(BitMap *bm, uint32_t size) {
    uint32_t byte_size = size + sizeof(uint32_t);
    uint32_t bit_size = byte_size / bm->ratio;
    if(byte_size % bm->ratio != 0) {
        bit_size ++;
    }

    uint8_t *ptr = bm->bitmap;
    uint8_t *end = ptr + bm->bitmap_size;

    uint32_t bit_cnt = 0;
    uint8_t *start = 0;
    uint8_t start_i = 0;
    while(ptr != end) {
        for(int i = 0; i < 8; ++i) {
            if(!(*ptr & (1 << i))) {
                if(bit_cnt == 0) {
                    start = ptr;
                    start_i = i;
                }
                bit_cnt ++;
            } else {
                bit_cnt = 0;
            }
            if(bit_cnt == bit_size) {
                uint8_t *tmp = start;
                uint32_t bit_cnt_down = bit_cnt;
                uint8_t s1 = min(8, bit_cnt_down + start_i);


                for(int j = start_i; j < s1; ++j) {
                    *tmp = *tmp | (1 << j);
                }
                bit_cnt_down -= (s1 - start_i);
                tmp++;

                uint32_t times = bit_cnt_down / 8;
                if(times) {
                    memset(tmp, 0xff, times);
                    tmp += times;
                }
                uint32_t res = bit_cnt_down % 8;

                for(int j = 0; j < res; ++j) {
                    *tmp = *tmp | (1 << j);
                }

                void *ret = bm->managed + (((void*)start - bm->bitmap)*8 + start_i) * bm->ratio;
                *(uint32_t *)ret = bit_size;
                return ret + sizeof(uint32_t);
            }
        }
        ++ptr;
    }

    return NULL;

}
/**
 * \fn void bitmap_free(BitMap *bm, void *addr);
 * \brief free a piece of memroy
 * \param pm BitMap struct pointer.
 * \param addr the memroy you want to free.
 *  it firstly dereference (addr - 4) to get size(in bit) of the memory
 *  then clear the bits : [bit_offset, bit_offset + bit_size].
 */

void bitmap_free(BitMap *bm, void *addr) {
    uint32_t bit_size = *(uint32_t*)(addr - 4);
    uint32_t bit_offset = (addr - bm->managed - 4) / bm->ratio;
    
    uint32_t times = bit_offset / 8;
    uint32_t res = bit_offset % 8;

    uint8_t *ptr = bm->bitmap + times;

    uint8_t s1 = min(8, bit_size + res);

    for(int i = res; i < s1; ++i) {
        *ptr = *ptr & (~(1 << i));
    }
    ++ptr;
    bit_size -= (s1 - res);

    uint32_t times2 = bit_size / 8;
    uint32_t res2 = bit_size % 8;

    memset(ptr, 0, times2);
    ptr += times2;

    for(int i = 0; i < res2; ++i) {
        *ptr = * ptr & (~(1 << i));
    }
}