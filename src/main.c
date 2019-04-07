#include "multiboot.h"
#include "vga.h"
#include "string.h"
#include "segment.h"
#include "interrupt.h"
#include "keyboard.h"
#include "debug.h"
#include "istream.h"
#include "pmap.h"
#include <stddef.h>
#include <stdint.h>

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
	vga_clear();
	vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	debug_printf("Magic number: %x\n", magic);
	
	debug_printf("Detecting memory.\n");
	pmap_init(mbd);
	debug_printf("Done.\n");
	
	debug_printf("Initializing segments.\n");
	segment_init();
	debug_printf("Done.\n");
	
	debug_printf("Initializing interrupts.\n");
	interrupt_init();
	debug_printf("Done.\n");
	
	debug_printf("Initializing keyboard.\n");
	keyboard_init();
	debug_printf("Done.\n");
	
	char buf[100];
	while(1) {
		size_t len = istream_gets(buf, sizeof(buf));
		debug_printf("INPUT(%d): %s\n", len, buf);
	}
}
