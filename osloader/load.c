/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */
#include "../arch/x86/x86.h"


void osloader_main(void)
{
    int counter = 0;
    while(1)
    {
        counter++;
    }
}