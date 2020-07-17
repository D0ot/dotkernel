#ifndef LOAD_H_
#define LOAD_H_

#include <stdint.h>
#include "libs/mem.h"
#include "libs/paging.h"

typedef struct {
    MemoryRegion *mrs;
    uint32_t max_size_mr_index;
    PageDirectoryEntry *phy_pdes;
} KernelBootArgs;

#endif