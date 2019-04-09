#include "pmap.h"
#include "queue.h"
#include "string.h"
#include "debug.h"
#include <stdint.h>

#define PMAP_ALLOC_TABLE_SIZE 0x8000
#define PMAP_BLOCK_QUEUE_SIZE 0x8000
#define PMAP_FRAG_QUEUE_SIZE 0x8000

static uint32_t pmap_alloc_table[PMAP_ALLOC_TABLE_SIZE] = { [0 ... (PMAP_ALLOC_TABLE_SIZE - 1)] = 0xffffffff };
static uint16_t pmap_block_ptr;

static uint16_t pmap_block_queue_buffer[PMAP_BLOCK_QUEUE_SIZE];
static queue_t pmap_block_queue = QUEUE((void*)pmap_block_queue_buffer, PMAP_BLOCK_QUEUE_SIZE, 2);

static uint16_t pmap_frag_queue_buffer[PMAP_FRAG_QUEUE_SIZE];
static queue_t pmap_frag_queue = QUEUE((void*)pmap_frag_queue_buffer, PMAP_FRAG_QUEUE_SIZE, 2);

void pmap_init(multiboot_info_t* mbd)
{
	if(!(mbd->flags & 0x40))
	{
		debug_printf("Error: Multiboot bit 6 not set.\n");
		return;
	}
	
	debug_printf("Kernel ends at: %x\n", &_kernel_end);
	
	void* ptr = (void*)mbd->mmap_addr;
	void* end = ptr + mbd->mmap_length;
	
	while(ptr < end)
	{
		multiboot_memory_map_t* entry = (multiboot_memory_map_t*)ptr;
		debug_printf("Addr: %x Size: %x Type: %d\n", entry->addr_low, entry->len_low, entry->type);
		
		if(entry->type != MULTIBOOT_MEMORY_AVAILABLE)
			goto ignore_section;
		
		uint32_t addr = entry->addr_low;
		uint32_t len = entry->len_low;
		
		if(addr < (uint32_t)&_kernel_end)
		{
			if(addr + len < (uint32_t)&_kernel_end)
			{
				debug_printf("Section below kernel end. Ignoring.\n");
				goto ignore_section;
			}
			
			len -= (uint32_t)&_kernel_end - addr;
			addr = (uint32_t)&_kernel_end;
		}

		uint32_t addr_start = (addr + 0xfff) & 0xfffff000;
		len = (len - (addr_start - addr)) & 0xfffff000;
		if(!len)
		{
			debug_printf("Aligned section smaller than 4KB. Ignoring.\n");
			goto ignore_section;
		}
		
		uint32_t addr_end = addr_start + len;
		
		uint32_t page_start = addr_start >> 12;
		uint32_t page_end = addr_end >> 12;
		
		uint16_t block_start = page_start >> 5;
		uint16_t block_end = page_end >> 5;
		
		uint8_t page_start_offset = page_start & 0x1f;
		uint8_t page_end_offset = page_end & 0x1f;
		
		if(block_start != block_end)
		{
			uint32_t first_block = 0;
			for(uint8_t b = 0; b < page_start_offset; ++b)
			{
				first_block |= (1 << b);
			}
			pmap_alloc_table[block_start] &= first_block;
			
			uint32_t last_block = 0;
			for(uint8_t b = page_end_offset; b < 32; ++b)
			{
				last_block |= (1 << b);
			}
			pmap_alloc_table[block_end] &= last_block;
			
			for(uint16_t block = block_start + 1; block < block_end; ++block)
			{
				pmap_alloc_table[block] = 0;
			}
		} else {
			uint32_t block = 0;
			for(uint8_t b = page_start_offset; b < page_end_offset; ++b)
			{
				block |= (1 << b);
			}
			pmap_alloc_table[block] &= ~block;
		}
		
		if(pmap_alloc_table[block_start])
		{
			queue_push(&pmap_frag_queue, &block_start);
			if(block_end > block_start + 1)
			{
				uint16_t block = block_start + 1;
				queue_push(&pmap_block_queue, &block);
			}
		} else {
			queue_push(&pmap_block_queue, &block_start);
		}
		
		if(block_start != block_end)
		{
			if(pmap_alloc_table[block_end] != (uint32_t)0xffffffff)
			{
				queue_push(&pmap_frag_queue, &block_end);
			}
		}
		
		debug_printf("Start addr: %x Size: %x\n", addr_start, addr_end - addr_start);
		debug_printf("Start page: %d #Pages: %d\n", page_start, page_end - page_start);
		debug_printf("Start block: %d End block: %d\n", block_start, block_end);
		
		ignore_section:
		
		ptr += entry->size + 4;
	}
	
	queue_pop(&pmap_block_queue, &pmap_block_ptr);
	
	if(!queue_is_empty(&pmap_frag_queue))
	{
		uint16_t next_frag;
		queue_top(&pmap_frag_queue, &next_frag);
		debug_printf("Next frag is: %d(%x)\n", next_frag, pmap_alloc_table[next_frag]);
	}
	debug_printf("Next block is: %d(%x)\n", pmap_block_ptr, pmap_alloc_table[pmap_block_ptr]);
	
}

static const uint8_t pmap_nib_helper[15] = {0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};
static inline uint8_t pmap_get_free_page(uint32_t block)
{
	uint8_t index = 0;
	
	uint16_t half = (uint16_t)block;
	if(half == 0xffff)
	{
		half = (uint16_t)(block >> 16);
		index += 16;
	}
	
	uint8_t quarter = (uint8_t)half;
	if(quarter == 0xff)
	{
		quarter = (uint8_t)(half >> 8);
		index += 8;
	}
	
	uint8_t nib = quarter & 0xf;
	if(nib == 0xf)
	{
		nib = quarter >> 4;
		index += 4;
	}
	
	return index + pmap_nib_helper[nib];
}

static inline uint16_t pmap_request_block()
{
	uint32_t current_block = pmap_alloc_table[pmap_block_ptr];
	
	if(current_block != 0)
	{
		if(queue_is_empty(&pmap_block_queue))
			return 0;
		
		queue_pop(&pmap_block_queue, &pmap_block_ptr);
		
		return pmap_request_block();
	}
	
	if(pmap_block_ptr == PMAP_ALLOC_TABLE_SIZE - 1)
	{
		if(queue_is_empty(&pmap_block_queue))
			return pmap_block_ptr;
		
		uint16_t ret_block = pmap_block_ptr;
		queue_pop(&pmap_block_queue, &pmap_block_ptr);
		
		return ret_block;
	}
	
	return pmap_block_ptr++;
}

#define PMAP_PAGE_ADDR(block, page) (void*)((32 * block + page) << 12);

void* pmap_palloc()
{
	if(queue_is_empty(&pmap_frag_queue))
	{
		uint16_t new_block = pmap_request_block();
		if(!new_block)
			return (void*)0x0;
		
		pmap_alloc_table[new_block] = 0x1;
		queue_push(&pmap_frag_queue, &new_block);
		
		return PMAP_PAGE_ADDR(new_block, 0);
	}
	
	uint16_t current_frag_index;
	queue_top(&pmap_frag_queue, &current_frag_index);
	
	uint32_t current_frag = pmap_alloc_table[current_frag_index];
	
	if(current_frag == 0)
	{
		queue_pip(&pmap_frag_queue);
		return pmap_palloc();
	}
	
	uint8_t page_index = pmap_get_free_page(current_frag);
	
	current_frag |= (1 << page_index);
	pmap_alloc_table[current_frag_index] = current_frag;
	
	if(current_frag == 0xffffffff)
	{
		queue_pip(&pmap_frag_queue);
	}
	
	return PMAP_PAGE_ADDR(current_frag_index, page_index);
}

void pmap_pfree(void* addr)
{
	if(!addr)
		return;
	
	uint32_t page = (uint32_t)addr >> 12;
	uint16_t block = page >> 5;
	
	uint8_t page_offset = page & 0x1f;
	
	pmap_alloc_table[block] &= ~(1 << page_offset);
	
	if(pmap_alloc_table[block] == 0)
	{
		queue_push(&pmap_block_queue, &block);
	}
}
