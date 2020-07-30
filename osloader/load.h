#ifndef _LOAD_H_
#define _LOAD_H_

#include <stdint.h>
#include "libs/mem.h"
#include "libs/paging.h"

typedef struct {
    MemoryRegion *mrs;
    uint32_t mr_size;
    uint32_t mr_max_length_index;
    pde_t *pde_vaddr;
    void *next_free_vaddr;
    void *next_free_paddr;
} KernelBootArgs;

#endif