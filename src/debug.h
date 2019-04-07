#ifndef DEBUG_H
#define DEBUG_H

#include "segment.h"
#include <stdint.h>
#include <stddef.h>

void debug_putchar(char c);
void debug_printf(const char* fmt, ...);
void debug_print_gdt_entry(struct gdtr* reg, size_t index);

#endif
