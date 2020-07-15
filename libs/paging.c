#include "paging.h"
#include "libs/log.h"


void paging_set_pde_4m_addr(PageDirectoryEntry* pde, uint32_t addr, uint8_t pat) {
    pde->pde = (pde->pde & (0xfff)) | ((pat & 0x01) << 12) | (addr & 0xffc00000);
    LOG_INFO("pde : %x", pde->pde);
}

void paging_set_pde_table_addr(PageDirectoryEntry* pde, uint32_t addr) {
    pde->pde = (pde->pde & (0xfff)) | (addr & (~0xfff));
}

