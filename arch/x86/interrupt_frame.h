#ifndef _INTERRUPT_FRAME_H_
#define _INTERRUPT_FRAME_H_

#include <stdint.h>

typedef struct {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} InterruptFrame;

#endif
