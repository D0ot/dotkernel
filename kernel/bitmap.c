#include "bitmap.h"
#include "libs/common.h"
#include "libs/log.h"

// size must be a multiple of (8 * ratio)

void bitmap_init(BitMap *bm) {
    uint32_t bitmap_size = bm->managed_size / bm->ratio / 8;
    LOG_VAR(bitmap_size);
    bm->bitmap_size = bitmap_size;
    memset(bm->bitmap, 0, bitmap_size);
}

void* bitmap_alloc(BitMap *bm, uint32_t size) {
    uint32_t byte_size = size + sizeof(uint32_t);
    uint32_t bit_size = byte_size / bm->ratio;
    if(byte_size % bm->ratio != 0) {
        bit_size ++;
    }

    uint8_t *ptr = bm->bitmap;
    LOG_VAR(ptr);
    uint8_t *end = ptr + bm->bitmap_size;
    LOG_VAR(end);

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
            LOG_VAR(bit_cnt);
            if(bit_cnt == bit_size) {
                uint8_t *tmp = start;
                uint32_t bit_cnt_down = bit_cnt;
                uint32_t diff = 8 - start_i;
                uint8_t s1 = diff <= bit_cnt_down ? 8 : start_i + bit_cnt_down;
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
void bitmap_free(BitMap *bm, void *addr) {
    uint32_t bit_size = *(uint32_t*)(addr - 4);
    uint32_t bit_offset = (addr - bm->bitmap - 4) / bm->ratio;
    
    uint32_t times = bit_offset / 8;
    uint32_t res = bit_offset % 8;

    uint8_t *ptr = bm->bitmap + times;

    
    
}