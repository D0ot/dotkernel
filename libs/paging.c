#include "paging.h"
#include "libs/log.h"


void paging_set_pde_4m_addr(pde_t *pde, uint32_t addr, uint8_t pat) {
    pde->pde = (pde->pde & (0xfff)) | ((pat & 0x01) << 12) | (addr & 0xffc00000);
}

void paging_set_pde_table_addr(pde_t *pde, uint32_t addr) {
    pde->pde = (pde->pde & (0xfff)) | (addr & (~0xfff));
}

void paging_set_pte_addr(pte_t *pte, uint32_t addr) {
    pte->addr = (addr >> 12);
}

uint32_t paging_paddr_in_pdep(pde_t *pde) {
    return pde->pde & 0xffc00000;
}

uint32_t paging_paddr_in_pdet(pde_t *pde) {
    return pde->pde & 0xfffff000;
}

uint32_t paging_paddr_in_pte(pte_t *pte) {
    return pte->pte & 0xfffff000;
}