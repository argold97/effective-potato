#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdint.h>

struct __attribute__((packed)) gdtr {
		uint16_t limit;
		uint32_t base;
};

struct __attribute__((packed)) gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t limit_high_flags;
	uint8_t base_high;
};

enum gdt_index {
	GDT_INDEX_R0_CODE = 1,
	GDT_INDEX_R0_DATA = 2,
	GDT_INDEX_R3_CODE = 3,
	GDT_INDEX_R3_DATA = 4
};

void segment_init();

#endif
