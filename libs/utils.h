#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>
#include <stdint.h>

// if match, return 1
// if not match, return 0
uint8_t strmatch(const char *s1, const char *s2,  uint32_t count);

char* reverse(char *buffer, size_t i, size_t j);
uint8_t checksum(uint8_t *p, uint32_t count);
// if match, return 1
// if not match, return 0
uint8_t strmatch(const char *s1, const char *s2,  uint32_t count);

// find the next number which is times of n
uint32_t nextntimes(uint32_t num, uint32_t n);

uint32_t max(uint32_t a, uint32_t b);
uint32_t min(uint32_t a, uint32_t b);

char *itoa(int value, char *str, int base);
char *utoa(unsigned value, char *str, unsigned base);
void swap(char *x, char *y);
int abs(int value);

#define POWER_OF_2(X) ((1 << (X)))

#endif