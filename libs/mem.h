#ifndef MEM_H_
#define MEM_H_

#include <stdint.h>
#include <stddef.h>

typedef struct AddressRangeDescriptorStructure_tag
{
    uint32_t base_address_low;
    uint32_t base_address_high;

    uint32_t length_low;
    uint32_t length_high;

    uint32_t type;
    uint32_t acpi3_extended;

} __attribute__((packed)) Mem_SMAP_Entry;


// any Memory_Region is all usable.
typedef struct Memory_Region_tag
{
    // base address
    uint32_t base;  

    // length
    uint32_t length;
} Memory_Region;

#endif