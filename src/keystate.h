#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <stdint.h>

struct keystate_entry {
	uint8_t pressed: 1;
	uint8_t lower: 7;
	uint8_t upper;
};

void keystate_init();

void keystate_set_scan_set(uint8_t scan_set);

void keystate_process_scan_code(uint8_t val);

#endif
