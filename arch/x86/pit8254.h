#ifndef _PIT8254_H_
#define _PIT8254_H_

#include <stdint.h>

#define PIT_8254_DATA1      (0x40)
#define PIT_8254_DATA2      (0x41)
#define PIT_8254_DATA3      (0x42)
#define PIT_8254_COMMAND    (0x43)

#define PIT_8254_CHL        (6)
#define PIT_8254_ACC        (4)
#define PIT_8254_MOD        (1)
#define PIT_8254_BCD        (0)

void x86_pit8254_init(uint16_t freq);

#endif