#ifndef _LOAD_H_
#define _LOAD_H_

#include <stdint.h>
#include "libs/mem.h"
#include "libs/paging.h"

typedef struct {
    MemoryRegion *mrs;
    uint32_t mr_size;
    PageDirectoryEntry *pdes_paddr;
    void *next_free_vaddr;
} KernelBootArgs;

#endif