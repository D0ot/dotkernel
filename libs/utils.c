#include "libs/utils.h"
#include "libs/kustd.h"

uint8_t strmatch(const char *s1, const char *s2,  uint32_t count)
{
    for(uint32_t i = 0; i != count; ++i)
    {
        if(*(s1+i) != *(s2+i))
        {
            return 0;
        }
        
    }
    return 1;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, size_t i, size_t j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

uint8_t checksum(uint8_t *p, uint32_t count)
{
    uint8_t sum = 0; // the checksum value
    for(uint8_t *iter_ptr = p; iter_ptr != p + count; ++iter_ptr)
    {
        sum += *iter_ptr;
    }

    return sum;
}

uint32_t nextntimes(uint32_t num, uint32_t n)
{ 
    return num + n - (num % n);
}


uint32_t max(uint32_t a, uint32_t b) {
    if(a > b) {
        return a;
    } else {
        return b;
    }
}
uint32_t min(uint32_t a, uint32_t b) {
    if(a > b) {
        return b;
    } else {
        return a;
    }
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	size_t i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

char *utoa(unsigned value, char* buffer, unsigned base)
{
    if(base < 2 || base > 32)
    {
        return buffer;
    }

    unsigned n = value;
    size_t i = 0;
    while(n)
    {
        unsigned r = n % base;
        if(r >= 10)
        {
            buffer[i++] = 65 + (r - 10);
        }
        else
        {
            buffer[i++] = 48 + r;
        }
        n = n / base;
    }
    
    if(i == 0)
        buffer[i++] = '0';
    
    buffer[i] = '\0';
    return reverse(buffer, 0, i - 1);
}


void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}


int abs(int value)
{
    return value > 0 ? value : -value;
}