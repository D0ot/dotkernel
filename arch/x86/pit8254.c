#include "pit8254.h"
#include "x86.h"
#include "libs/log.h"

#define test         (0 << PIT_8254_CHL) | (3 << PIT_8254_ACC) | (2 << PIT_8254_MOD) | (0 << PIT_8254_BCD)
 

void x86_pit8254_init(uint16_t freq) {
    
    uint16_t reload = 1193180 / freq;

    // ctrl bit
    x86_outb(PIT_8254_COMMAND,  
        (0 << PIT_8254_CHL) |
        (3 << PIT_8254_ACC) |
        (2 << PIT_8254_MOD) |
        (0 << PIT_8254_BCD) 
    );


    // set low bit
    x86_outb(PIT_8254_DATA1, (uint8_t)reload);

    // set high bit
    x86_outb(PIT_8254_DATA1, (uint8_t)(reload >> 8));
}
