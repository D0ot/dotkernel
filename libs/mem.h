#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>
#include <stdint.h>

typedef struct AddressRangeDescriptorStructure_tag {
    uint32_t base_address_low;
    uint32_t base_address_high;

    uint32_t length_low;
    uint32_t length_high;

    uint32_t type;
    uint32_t acpi3_extended;

} __attribute__((packed)) Mem_SMAP_Entry;

#define SMAP_TYPE_USABLE (1)

// any MemoryRegion is all usable.
typedef struct MemoryRegion_tag {
    // base address
    uint32_t base;

    // length
    uint32_t length;
} MemoryRegion;

#endif