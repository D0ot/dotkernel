#include "interrupt_frame.h"


extern void *x86_irq_sub_handle_ptr[256];

__attribute__ ((interrupt))
void x86_irq0_handle(InterruptFrame *frame) {
    
}


