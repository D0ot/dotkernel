#include "uart.h"
#include "libs/common.h"
#include "libs/disk.h"

/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */

void putchar(char c) {
    uart_putchar(c);
}

void osloader_main(void)
{
    uint8_t buf[8192];
    const int offset = 512 * 6;
    for(int i = 0; i < 4; ++i) {
        buf[offset + i] = 0xff;
    }
    disk_read(0, 6, (uint16_t*)buf);
    while(1);
}

