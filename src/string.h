#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* str, int c, size_t n);
char* itoa(int val, char* str);
char* itoa_hex(unsigned int val, char* str);

#endif