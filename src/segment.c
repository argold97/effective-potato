#include "segment.h"

static struct gdt_entry gdt[5] = {
	{ 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00 },
	{ 0xffff, 0x0000, 0x00, 0x9a, 0xcf, 0x00 },
	{ 0xffff, 0x0000, 0x00, 0x92, 0xcf, 0x00 },
	{ 0xffff, 0x0000, 0x00, 0xfA, 0xcf, 0x00 },
	{ 0xffff, 0x0000, 0x00, 0xf2, 0xcf, 0x00 }
};

void segment_init()
{
	struct gdtr reg = { (sizeof(gdt) / sizeof(struct gdt_entry)) << 3, (uint32_t)gdt };
	
	asm volatile(
		"lgdt %0\n"
		"mov $0x10, %%eax\n"
		"mov %%eax, %%ds\n"
		"mov %%eax, %%ss\n"
		"mov %%eax, %%es\n"
		"mov %%eax, %%fs\n"
		"mov %%eax, %%gs\n"
		"ljmp $0x08, $lbl\n"
		"lbl: \n"
		:
		: "m" (reg)
		: "eax"
	);
}