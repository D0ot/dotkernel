#include "libs/log.h"
#include "libs/kustd.h"
#include "libs/uart.h"
#include <stdarg.h>


void klog_putchar(char ch)
{
    uart_putchar(ch);
}

void klog(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    v_printf_callback(format, klog_putchar, ap);
    va_end(ap);
}

void klog_level(int level, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    if(level < 0 || level > 3)
    {
        while(1)
        {
            // usage error.
        }
    }
    char *level_str[4] = {"Debug", "Info", "Warning", "Error"};
    char *output = level_str[level];
    while(*output)
    {
        klog_putchar(*output++);
    }
    output = " | ";
    while (*output)
    {
        klog_putchar(*output++);
    }
    
    v_printf_callback(format, klog_putchar, ap);
    va_end(ap);
}