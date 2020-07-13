#include "libs/uart.h"
#include "libs/common.h"
#include "libs/disk.h"
#include "libs/elf.h"
#include "libs/mem.h"

/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */
void memory_init() {
    
}



void osloader_main(void) {
    printf_func("enter osloader_main", uart_putchar);
}

