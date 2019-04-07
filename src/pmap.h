#ifndef PMAP_H
#define PMAP_H

#include "multiboot.h"

extern const void _kernel_end;

void pmap_init(multiboot_info_t* mbd);

void* pmap_palloc();

void pmap_pfree(void* addr);

#endif
