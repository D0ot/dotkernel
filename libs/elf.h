#ifndef _ELF_H_
#define _ELF_H_

#include <stdint.h>
#include <stddef.h>

typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef uint32_t Elf32_Sword;
typedef uint32_t Elf32_Word;


#define EI_NIDENT   16
typedef struct {
    unsigned char       e_ident[EI_NIDENT];
    Elf32_Half          e_type;
    Elf32_Half          e_machine;
    Elf32_Word          e_version;
    Elf32_Addr          e_entry;
    Elf32_Off           e_phoff;
    Elf32_Off           e_shoff;
    Elf32_Word          e_flags;
    Elf32_Half          e_ehsize;
    Elf32_Half          e_phentsize;
    Elf32_Half          e_phnum;
    Elf32_Half          e_shentsize;
    Elf32_Half          e_shnum;
    Elf32_Half          e_shstrndx;
} Elf32_Ehdr;


// e_ident

#define EI_MAG0         0
#define ELFMAG0         0x7f

#define EI_MAG1         1
#define ELFMAG1         'E'

#define EI_MAG2         2
#define ELFMAG2         'L'

#define EI_MAG3         3
#define ELFMAG3         'F'

#define EI_CLASS        4
#define ELFCLASSNONE    0
#define ELFCLASS32      1
#define ELFCLASS64      2

#define EI_DATA         5
#define ELFDATANONE     0
#define ELFDATA2LSB     1
#define ELFDATA2MSB     2

#define EI_VERSION      6

#define EI_PAD          7


// e_type
#define ET_NONE         0
#define ET_REL          1
#define ET_EXEC         2
#define ET_DYN          3
#define ET_CORE         4

// e_machine
#define EM_386          3


// e_version
#define EV_CURRENT      1

typedef struct {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

#define SHT_NULL        0
#define SHT_PROGBITS    1

typedef struct {
    Elf32_Word  p_type;
    Elf32_Off   p_offset;
    Elf32_Addr  p_vaddr;
    Elf32_Addr  p_paddr;
    Elf32_Word  p_filesz;
    Elf32_Word  p_memsz;
    Elf32_Word  p_flags;
    Elf32_Word  p_align;
} Elf32_Phdr;

// p_type
#define PT_NULL         0
#define PT_LOAD         1
#define PT_DYNAMIC      2


#endif