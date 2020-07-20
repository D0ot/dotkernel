#ifndef _KDEBUG_H
#define _KDEBUG_H

#include <stdarg.h>

enum 
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};

#define LOG_LEVEL(...)                          \
    do                                          \
    {                                           \
        klog("%s:%d | ", __FILE__, __LINE__);    \
        klog_level(__VA_ARGS__);                 \
        klog_putchar('\n');                      \
    }while(0)


#define LOG_DEBUG(...) LOG_LEVEL(LOG_LEVEL_DEBUG, __VA_ARGS__);
#define LOG_INFO(...) LOG_LEVEL(LOG_LEVEL_INFO, __VA_ARGS__);
#define LOG_WARNING(...) LOG_LEVEL(LOG_LEVEL_WARNING, __VA_ARGS__);
#define LOG_ERROR(...) LOG_LEVEL(LOG_LEVEL_ERROR, __VA_ARGS__);

#define LOG_ASSERT(x)                           \
    do                                          \
    {                                           \
        if(!(x)) {                                \
            LOG_ERROR("assert \"" #x "\" failed.");\
        }                                       \
    }while (0)

#define LOG_VAR(x)                              \
    do                                          \
    {                                           \
        LOG_INFO("%s = %x", #x, x);             \
    }while(0)
    

void klog_putchar(char ch);
void klog(const char *format, ...);
void klog_level(int level, const char *format, ...);

#endif 