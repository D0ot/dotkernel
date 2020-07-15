#ifndef PAGING_H_
#define PAGING_H_

#include <stddef.h>
#include <stdint.h>

#define PAGING_PDE_P        (1 << 0)
#define PAGING_PDE_RW       (1 << 1)
#define PAGING_PDE_US       (1 << 2)
#define PAGING_PDE_PWT      (1 << 3)
#define PAGING_PDE_PCD      (1 << 4)
#define PAGING_PDE_PS       (1 << 7)
#define PAGING_PDE_G        (1 << 8)
#define PAGING_PDE_PAT      (1 << 12)

typedef struct {
    union {
        struct {
            uint8_t p : 1;   // present
            uint8_t rw : 1;  // read/write
            uint8_t us : 1;  // user/supervisor
            uint8_t pwt : 1; // page-level write-through
            uint8_t pcd : 1; // page-level cache disable
            uint8_t a : 1;   // accessed
            uint8_t d : 1;   // dirty
            uint8_t ps : 1;  // 1 for 4MB page, 0 for page directory
            uint8_t g : 1;   // global page
        };
        uint32_t pde;
    };
} __attribute__((packed)) PageDirectoryEntry;



void paging_set_pde_table_addr(PageDirectoryEntry* pde, uint32_t addr);
void paging_set_pde_4m_addr(PageDirectoryEntry* pde, uint32_t addr, uint8_t pat);






#endif