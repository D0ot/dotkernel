#include "defs/defs.h"
#include "libs/common.h"
#include "libs/disk.h"
#include "libs/elf.h"
#include "libs/log.h"
#include "libs/mem.h"
#include "libs/uart.h"
#include "libs/paging.h"

/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */

typedef struct {
    MemoryRegion *mrs;
    uint32_t max_size_mr_index;
}KernelBootArgs;

void memory_pre_process(MemoryRegion ** pmrs, uint32_t* max_index)
{
    Mem_SMAP_Entry *smap = (void *)SMAP_ADDR + 4;
    int size = *(int *)(SMAP_ADDR);
    MemoryRegion * mrs = (void*)(smap) + size * SMAP_ENTRY_SIZE;
    LOG_INFO("Total memory info");
    int cnt = 0;
    for (int i = 0; i < size; ++i) {
        LOG_INFO("%dth entry in smap: %x_%x, %x_%x, %x, %x", i,
                 smap[i].base_address_high,
                 smap[i].base_address_low,
                 smap[i].length_high,
                 smap[i].length_low,
                 smap[i].type,
                 smap[i].acpi3_extended);
        if(smap[i].type == SMAP_TYPE_USABLE && smap[i].acpi3_extended == 1)
        {
            mrs[cnt].base = smap[i].base_address_low;
            mrs[cnt].length = smap[i].length_low;
            cnt++;
        }
    }
    int maxsize_index = 0;
    LOG_INFO("Usuable memroy info")
    for(int i = 0; i < cnt; ++i) {
        LOG_INFO("%dth usuable memroy region, %x, %x", i, mrs[i].base, mrs[i].length);
        if(mrs[i].length > mrs[maxsize_index].length) {
            maxsize_index = i;
        }
    }
    LOG_INFO("Max-size usuable memroy region, %x, %x",mrs[maxsize_index].base, mrs[maxsize_index].length);
    *max_index = maxsize_index;
    *pmrs = mrs;
}

void set_up_paging(MemoryRegion *mr) {
    uint32_t first4m = (4 << 20) - (mr->base % (4 << 20)) + mr->base ;
    PageDirectoryEntry *pde = (PageDirectoryEntry*)(first4m + (16 << 20));

    memset((void*)pde, 0, 4 << 20);
    // setup pde points to pde
    pde[0x3ff].pde = PAGING_PDE_P | PAGING_PDE_RW | PAGING_PDE_US | PAGING_PDE_PCD | PAGING_PDE_PWT;
    paging_set_pde_4m_addr(pde + 0x3ff, (uint32_t)pde, 0);

    // setup the first PDE
    pde[0].pde = PAGING_PDE_P | PAGING_PDE_RW | PAGING_PDE_US | PAGING_PDE_PCD | PAGING_PDE_PWT;
    paging_set_pde_4m_addr(pde, 0x0, 0);

    PageDirectoryEntry tmp;
    tmp.pde = PAGING_PDE_P | PAGING_PDE_RW | PAGING_PDE_US | PAGING_PDE_PCD | PAGING_PDE_PWT;

    // setup PDE for kernel to load
    
    for(int i = 0; i < 4; ++i) {
        pde[0x200 + i] = tmp;
        paging_set_pde_4m_addr(pde + 0x200 + i, first4m + (4 << 20) * i,0);
    }
}




void osloader_main(void)
{
    LOG_INFO("osloader_main, enter");
    void (*kmain)(KernelBootArgs args);
    kmain = (void*)KERNEL_START;
    KernelBootArgs kba;
    memory_pre_process(&(kba.mrs),&(kba.max_size_mr_index));
    set_up_paging(kba.mrs + kba.max_size_mr_index);
    //kmain(kba);
}
    
