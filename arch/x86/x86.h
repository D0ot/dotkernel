#ifndef _X86_H
#define _X86_H

#include <stdint.h>


static inline uint8_t x86_inb(uint16_t port)
{
    uint8_t data;
    __asm__ volatile ("in %0, %1;" : "=a"(data) : "d"(port));
    return data;
}

static inline void x86_outb(uint16_t port, uint8_t data)
{
    __asm__ volatile ("out %1, %0;" : : "a"(data), "d"(port));
}

static inline uint16_t x86_inw(uint16_t port)
{
    uint16_t data;
    __asm__ volatile ("in %0, %1;": "=a"(data) : "d"(port));
    return data;
}

static inline void x86_outw(uint16_t port, uint16_t data)
{
    __asm__ volatile ("out %1, %0;" : : "a"(data), "d"(port));
}

static inline void x86_insw(uint16_t port, uint16_t *buf, uint32_t count_byte)
{
    __asm__ volatile (
        "cld\n\t"
        "rep insw\n\t"
        : "=D"(buf), "=c"(count_byte) 
        : "d"(port), "0"(buf), "1"(count_byte)
        : "memory", "cc"
    );
}

static inline void x86_lgdt(void *addr)
{
    __asm__ volatile (
        "lgdt [%0]"
        : /* No output */
        : "a"(addr)
    );
}


typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
} cpu_id_output;

static inline void x86_cpuid(cpu_id_output*output) {
    __asm__ (
        "cpuid\n\t"
        : "+a"(output->eax), "=b"(output->ebx), "=c"(output->ecx), "=d"(output->edx)
    );
};

#define X86_BTS(OP1, OP2) __asm__("bts %[op1], %[op2]" : [op1]"+m"(OP1) : [op2]"rN"(OP2) : "cc")
#define X86_BTC(OP1, OP2) __asm__("btc %[op1], %[op2]" : [op1]"+m"(OP1) : [op2]"rN"(OP2) : "cc")

static inline void x86_mov_stack(void *new_stack, void *old_stack) {
    __asm__ (
        "mov eax, %[old]\n\t"
        "sub eax, esp\n\t"
        "mov ebx, %[new]\n\t"
        "sub ebx, eax\n\t"
        "mov esp, ebx\n\t"
        "xor edx, edx\n\t"
        "mov ebx, 4\n\t"
        "div ebx\n\t"
        "cmp edx, 0\n\t"
        "je .%=equal\n\t"
        "inc eax\n\t"
        ".%=equal:\n\t"
        "mov ecx, eax\n\t"
        "std\n\t"
        "mov edi, %[new]\n\t"
        "sub edi, 4\n\t"
        "mov esi, %[old]\n\t"
        "sub esi, 4\n\t"
        "rep movsd"
        : 
        : [new]"m"(new_stack), [old]"m"(old_stack)
        : "cc", "memory", "eax","ebx", "ecx", "edx", "esi", "edi"
    );
}


#define CR0_PG_BIT_OFFSET (31)

static inline uint32_t x86_read_cr0() {
    uint32_t out;
    __asm__ (
        "mov %[out], cr0"
        : [out] "=Q"(out)
    );
    return out;
}

static inline void x86_write_cr0(uint32_t in) {
    __asm__ (
        "mov cr0, %[in]"
        :
        : [in] "Q"(in)
    );
}

static inline uint32_t x86_read_cr3() {
    uint32_t out;
    __asm__ (
        "mov %[out], cr3"
        : [out] "=Q"(out)
    );
    return out;
}

static inline void x86_write_cr3(uint32_t in) {
    __asm__ (
        "mov cr3, %[in]"
        :
        : [in] "Q"(in)
    );
}



#define CR4_PSE_BIT_OFFSET (4)

static inline uint32_t x86_read_cr4() {
    uint32_t out;
    __asm__ (
        "mov %[out], cr4"
        : [out] "=Q"(out)
    );
    return out;
}

static inline void x86_write_cr4(uint32_t in) {
    __asm__ (
        "mov cr4, %[in]"
        :
        : [in] "Q"(in)
    );
}

#endif