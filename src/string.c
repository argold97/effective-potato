#include "string.h"
#include <stdbool.h>

void* memcpy(void* dest, const void* src, size_t n)
{
	char* ptr = dest;
	while(n--)
		*(char*)ptr++ = *(char*)src++;
	
	return dest;
}

void* memset(void* str, int c, size_t n)
{
	char* ptr = (char*)str;
	while(n--)
		*ptr++ = (char)c;
	
	return str;
}

char* itoa(int val, char* str)
{
	char* ptr = str;
	bool first = false;
	long unsigned int base = 1000000000;
	
	if(val < 0)
	{
		*ptr++ = '-';
		val = -val;
	}
	
	while(1)
	{
		int c = (unsigned int)val / base;
		val -= c * base;
		if(c || first) {
			first = true;
			*ptr++ = '0' + c;
		}
		base /= 10;
		if(base == 0)
			break;
	}
	if(!first)
		*ptr++ = '0';
	*ptr = '\0';
	return str;
}

char* itoa_hex(unsigned int val, char* str)
{
	inline char h_to_c(char c) {
		return (c > 9) ? c + 0x37 : c + '0';
	}
	
	for(char* ptr = str + 7; ptr >= str; --ptr)
	{
		*ptr = h_to_c(val & 0xF);
		val = val >> 4;
	}
	str[8] = '\0';
	
	return str;
}
