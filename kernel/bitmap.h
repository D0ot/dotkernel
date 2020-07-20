#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stddef.h>
#include <stdint.h>

typedef struct{
    void *managed;
    uint32_t managed_size;
    void *bitmap;
    uint32_t bitmap_size;
    uint32_t ratio;
} BitMap;


void bitmap_init(BitMap *bm, void*managed, void*bitmap, uint32_t managed_size, uint32_t ratio);
void* bitmap_alloc(BitMap *bm, uint32_t size);
void bitmap_free(BitMap *bm, void *addr);

void bitmap_set_clear(BitMap *bm, uint32_t bit_start, uint32_t bit_end, uint8_t value);


#endif