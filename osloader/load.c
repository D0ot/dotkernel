#include "defs/defs.h"
#include "libs/kustd.h"
#include "libs/utils.h"
#include "libs/disk.h"
#include "libs/elf.h"
#include "libs/log.h"
#include "libs/mem.h"
#include "libs/uart.h"
#include "libs/paging.h"
#include "arch/x86/x86.h"
#include "osloader/load.h"

/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */

typedef void(*kernel_start_fun)();

void osloader_memory_pre_process(MemoryRegion **pmrs, uint32_t *max_index, uint32_t *max_length_index)
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
    *max_index = cnt;
    int maxsize_index = 0;
    LOG_INFO("Usuable memroy info")
    for(int i = 0; i < cnt; ++i) {
        LOG_INFO("%dth usuable memroy region, %x, %x", i, mrs[i].base, mrs[i].length);
        if(mrs[i].length > mrs[maxsize_index].length) {
            maxsize_index = i;
        }
    }
    LOG_INFO("Max-size usuable memroy region, %x, %x",mrs[maxsize_index].base, mrs[maxsize_index].length);
    *max_length_index = maxsize_index;
    *pmrs = mrs;
}

void osloader_feature_check() {
    //check PSE, page size extension
    cpu_id_output tmp;
    tmp.eax = 0;
    x86_cpuid(&tmp);
    char buf[13];
    buf[sizeof(buf) - 1] = 0;
    memcpy(buf, &(tmp.ebx), 4);
    memcpy(buf + 4, &(tmp.ecx), 4);
    memcpy(buf + 8, &(tmp.edx), 4);
    LOG_INFO("cpuid.00h: eax : %x, manufacturer ID strings : %s", tmp.eax, buf);

    tmp.eax = 1;
    x86_cpuid(&tmp);
    if(tmp.edx & (1 << 3)) {
        LOG_INFO("cpuid.01h: PSE avaliable.");
    }else {
        LOG_INFO("cpuid.01h: eax : %x, ebx : %x, ecx : %x, edx : %x", tmp.eax, tmp.ebx, tmp.ecx, tmp.edx);
        LOG_ERROR("cpuid.01h: PSE not avaliable");
        while(1);
    }
}

pde_t* osloader_set_up_paging(MemoryRegion *mr) {
    uint32_t first4m = (4 << 20) - (mr->base % (4 << 20)) + mr->base;
    pde_t *pde = (pde_t*)(first4m + (KERNEL_INITIAL_4MPAGE_NUM << 22));

    // clear the pdes
    memset((void*)pde, 0, 4 << 10);

    // setup pde points to pde
    pde[0x3ff].pde = PAGING_PDE_P | PAGING_PDE_RW | PAGING_PDE_US | PAGING_PDE_PCD | PAGING_PDE_PWT | PAGING_PDE_PS;
    paging_set_pde_4m_addr(pde + 0x3ff, (uint32_t)pde, 0);

    // setup the first PDE
    pde[0].pde = PAGING_PDE_P | PAGING_PDE_RW | PAGING_PDE_US | PAGING_PDE_PCD | PAGING_PDE_PWT | PAGING_PDE_PS;
    paging_set_pde_4m_addr(pde, 0x0, 0);

    pde_t tmp;
    tmp.pde = PAGING_PDE_P | PAGING_PDE_RW | PAGING_PDE_US | PAGING_PDE_PCD | PAGING_PDE_PWT | PAGING_PDE_PS;

    // setup PDE for kernel to load
    
    for(int i = 0; i < KERNEL_INITIAL_4MPAGE_NUM; ++i) {
        pde[0x200 + i] = tmp;
        paging_set_pde_4m_addr(pde + 0x200 + i, first4m + (4 << 20) * i,0);
    }

    // enable CR4.PSE
    uint32_t reg = x86_read_cr4();
    X86_BTS(reg, CR4_PSE_BIT_OFFSET);
    x86_write_cr4(reg);

    // set CR3
    reg = (uint32_t)pde | (1 << 3) | (1 << 4);
    x86_write_cr3(reg);

    // set CR0.PG
    reg = x86_read_cr0();
    X86_BTS(reg, CR0_PG_BIT_OFFSET);
    x86_write_cr0(reg);

    return (pde_t*)(0xffffffff - ((4 << 20) - 1));
}


void osloader_load_kernel(void *free_buf, kernel_start_fun *entry, void** next_free_addr) {
    uint8_t *buf = free_buf;
    disk_read(KERNEL_SECTOR_BASE, 1, (uint16_t*)buf);

    Elf32_Ehdr ehdr;
    memcpy(&ehdr, buf, sizeof(ehdr));

    // magic number check
    LOG_ASSERT(ehdr.e_ident[EI_MAG0] == ELFMAG0);
    LOG_ASSERT(ehdr.e_ident[EI_MAG1] == ELFMAG1);
    LOG_ASSERT(ehdr.e_ident[EI_MAG2] == ELFMAG2);
    LOG_ASSERT(ehdr.e_ident[EI_MAG3] == ELFMAG3);

    
    LOG_ASSERT(ehdr.e_ident[EI_CLASS] == ELFCLASS32);
    LOG_ASSERT(ehdr.e_ident[EI_DATA] == ELFDATA2LSB);
    LOG_ASSERT(ehdr.e_ident[EI_VERSION] == EV_CURRENT);

    LOG_ASSERT(ehdr.e_type == ET_EXEC);
    LOG_ASSERT(ehdr.e_machine == EM_386);
    LOG_ASSERT(ehdr.e_version == EV_CURRENT);
    LOG_ASSERT(ehdr.e_ehsize == sizeof(ehdr));
    
    Elf32_Off phoff = ehdr.e_phoff;
    // skip, to skip 'skip' sectors
    // res, offset from the first sector
    // length, size in bytes
    // sector_num, number of sectors to read
    uint32_t res, skip, size, sector_num;
    skip = phoff / SECTOR_SIZE;
    res = phoff % SECTOR_SIZE;
    size = ehdr.e_phentsize * ehdr.e_phnum;
    sector_num = size / SECTOR_SIZE + 1;
    if(sector_num > 256) {
        sector_num = 0;
        LOG_WARNING("sector_num > 256");
    }

    // read program header
    const uint32_t OSLOADER_PHDR_MAX_NUM = 16;
    Elf32_Phdr phdr[OSLOADER_PHDR_MAX_NUM];
    uint32_t phnum_max = ehdr.e_phnum > OSLOADER_PHDR_MAX_NUM ? OSLOADER_PHDR_MAX_NUM : ehdr.e_phnum;

    disk_read(KERNEL_SECTOR_BASE + skip, sector_num, (uint16_t*)buf);
    memcpy(phdr, buf + res, sizeof(Elf32_Phdr) * phnum_max);


    *(uint32_t*)next_free_addr = 0;

    for(uint32_t i = 0; i < phnum_max; ++i) {
        LOG_ASSERT(size < (3 << 20));
        // load loadable segment into memory
        if(phdr[i].p_type == PT_LOAD) {
            size = phdr[i].p_memsz;
            skip = phdr[i].p_offset / SECTOR_SIZE;
            res = phdr[i].p_offset % SECTOR_SIZE;
            sector_num = size / SECTOR_SIZE + 1;
            disk_read_ex(KERNEL_SECTOR_BASE + skip, sector_num, (uint16_t*)buf);
            memcpy((void*)phdr[i].p_vaddr, buf + res, size);
            if(*(uint32_t*)next_free_addr < phdr[i].p_vaddr + size) {
                *(uint32_t*)next_free_addr = phdr[i].p_vaddr + size;
            }
        }
    }
    LOG_INFO("kernel entry : %x", ehdr.e_entry);
    *entry = (kernel_start_fun)ehdr.e_entry;
}

void osloader_main(void)
{
    LOG_INFO("osloader_main, enter");
    KernelBootArgs kba;
    kernel_start_fun kentry;
    osloader_memory_pre_process(&(kba.mrs), &(kba.mr_size), &(kba.mr_max_length_index));
    osloader_feature_check();
    kba.pde_vaddr = osloader_set_up_paging(kba.mrs + kba.mr_size);
    osloader_load_kernel((void*)kba.mrs->base, &kentry, &kba.next_free_vaddr);
    // 16MiB for loading kernel, 4Mib for PDEs(currently the last 4KiB is used)
    kba.next_free_paddr = (void*)( align_to(kba.mrs[kba.mr_max_length_index].base, POWER_OF_2(22)) + (4 << 20) * 4);
    LOG_INFO("osloader_main, leave, jmp to kentry");
    memcpy((void*)(KERNEL_BOOT_ARGS_ADDR), (void*)&kba, sizeof(kba));
    kentry();
}