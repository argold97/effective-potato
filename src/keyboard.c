#include "keyboard.h"
#include "debug.h"
#include "io.h"
#include "keystate.h"
#include "mutex.h"

static uint8_t keyboard_command_mode = 0;

static inline void keyboard_out(uint8_t val)
{
	while(io_inb(0x64) & 0x2)
		continue;
	
	io_outb(0x60, val);
	io_wait();
}

static inline uint8_t keyboard_in()
{
	while((io_inb(0x64) & 0x1) == 0)
		continue;
	
	uint8_t val = io_inb(0x60);
	io_wait();
	return val;
}

void keyboard_recv()
{
	if(keyboard_command_mode)
		return;
	
	uint8_t val = io_inb(0x60);
	
	keystate_process_scan_code(val);
}

static uint8_t keyboard_get_scan_set()
{
	keyboard_command_mode = 1;
	
	keyboard_out(0xF0);
	keyboard_in();
	
	keyboard_out(0);
	keyboard_in();
	
	uint8_t set = keyboard_in();
	keyboard_command_mode = 0;
	
	switch(set) {
		case 0x43:
			return 1;
		case 0x41:
			return 2;
		case 0x3f:
			return 3;
		default:
			return 0;
	}
}

static void keyboard_set_scan_set(uint8_t scan_set)
{
	if(scan_set == 0 || scan_set > 3)
		return;
	
	mutex_spinlock(&keyboard_command_mode);
	
	keyboard_out(0xF0);
	keyboard_in();
	
	keyboard_out(scan_set);
	keyboard_in();
	
	mutex_unlock(&keyboard_command_mode);
}

void keyboard_init()
{
	keystate_init();
	
	keyboard_set_scan_set(2);
	keystate_set_scan_set(1);
	
	debug_printf("Current scan set: %d\n", 1);
}
