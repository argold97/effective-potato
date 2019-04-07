#include "keystate.h"
#include "qwerty.h"
#include "string.h"
#include "istream.h"
#include "debug.h"

enum keystate_scan_state {
	KEYSTATE_SCAN_STATE_DEFAULT = 0,
	KEYSTATE_SCAN_STATE_RELEASE = 1,
	KEYSTATE_SCAN_STATE_MEDIA = 2,
	KEYSTATE_SCAN_STATE_MEDIA_RELEASE = 3
};

struct keystate_entry keystate_state_table[110];

static uint8_t keystate_current_scan_state = KEYSTATE_SCAN_STATE_DEFAULT;

static uint8_t keystate_current_scan_set = 2;

void keystate_init()
{
	debug_printf("Current keyboard encoding: US-QWERTY\n");
	memcpy(keystate_state_table, querty_state_table, sizeof(keystate_state_table));
}

static void keystate_process_scan_code_1(uint8_t val);
static void keystate_process_scan_code_2(uint8_t val);

void keystate_process_scan_code(uint8_t val)
{
	switch(keystate_current_scan_set) {
		case 1:
			keystate_process_scan_code_1(val);
			break;
		case 2:
			keystate_process_scan_code_2(val);
			break;
		case 3:
			break;
	}
}

void keystate_set_scan_set(uint8_t scan_set)
{
	keystate_current_scan_set = scan_set;
	keystate_current_scan_state = KEYSTATE_SCAN_STATE_DEFAULT;
}

static inline void keystate_on_down(uint8_t key)
{
	keystate_state_table[key].pressed = 1;
	
	uint8_t c;
	if(keystate_state_table[QWERTY_LSHIFT].pressed || keystate_state_table[QWERTY_RSHIFT].pressed)
	{
		c = keystate_state_table[key].upper;
	} else {
		c = keystate_state_table[key].lower;
	}
	if(!c)
		return;
	
	istream_putc_nonblk(c);
}

static inline void keystate_on_up(uint8_t key)
{
	keystate_state_table[key].pressed = 0;
}

static const uint8_t keystate_scan_set_1_primary[0x60];
static const uint8_t keystate_scan_set_2_primary[0x90];

static void keystate_process_scan_code_1(uint8_t val)
{
	switch(keystate_current_scan_state) {
		case KEYSTATE_SCAN_STATE_DEFAULT:
		{
			if(val == 0xE0) {
				keystate_current_scan_state = KEYSTATE_SCAN_STATE_MEDIA;
				return;
			}
			if(val > 0x80) {
				keystate_on_up(keystate_scan_set_1_primary[val - 0x80]);
			} else {
				keystate_on_down(keystate_scan_set_1_primary[val]);
			}
			return;
		}
		case KEYSTATE_SCAN_STATE_MEDIA:
		{
			keystate_current_scan_state = KEYSTATE_SCAN_STATE_DEFAULT;
			return;
		}
	}
}

static void keystate_process_scan_code_2(uint8_t val)
{
}

static const uint8_t keystate_scan_set_1_primary[0x60] = {
	[0x01] = QWERTY_ESC,
	[0x02] = QWERTY_1,
	[0x03] = QWERTY_2,
	[0x04] = QWERTY_3,
	[0x05] = QWERTY_4,
	[0x06] = QWERTY_5,
	[0x07] = QWERTY_6,
	[0x08] = QWERTY_7,
	[0x09] = QWERTY_8,
	[0x0A] = QWERTY_9,
	[0x0B] = QWERTY_0,
	[0x0C] = QWERTY_MINUS,
	[0x0D] = QWERTY_EQUALS,
	[0x0E] = QWERTY_BACKSPACE,
	[0x0F] = QWERTY_TAB,
	[0x10] = QWERTY_Q,
	[0x11] = QWERTY_W,
	[0x12] = QWERTY_E,
	[0x13] = QWERTY_R,
	[0x14] = QWERTY_T,
	[0x15] = QWERTY_Y,
	[0x16] = QWERTY_U,
	[0x17] = QWERTY_I,
	[0x18] = QWERTY_O,
	[0x19] = QWERTY_P,
	[0x1A] = QWERTY_LBRACKET,
	[0x1B] = QWERTY_RBRACKET,
	[0x1C] = QWERTY_ENTER,
	[0x1D] = QWERTY_LCTRL,
	[0x1E] = QWERTY_A,
	[0x1F] = QWERTY_S,
	[0x20] = QWERTY_D,
	[0x21] = QWERTY_F,
	[0x22] = QWERTY_G,
	[0x23] = QWERTY_H,
	[0x24] = QWERTY_J,
	[0x25] = QWERTY_K,
	[0x26] = QWERTY_L,
	[0x27] = QWERTY_SEMICOLON,
	[0x28] = QWERTY_QUOTE,
	[0x29] = QWERTY_BACKTICK,
	[0x2A] = QWERTY_LSHIFT,
	[0x2B] = QWERTY_BSLASH,
	[0x2C] = QWERTY_Z,
	[0x2D] = QWERTY_X,
	[0x2E] = QWERTY_C,
	[0x2F] = QWERTY_V,
	[0x30] = QWERTY_B,
	[0x31] = QWERTY_N,
	[0x32] = QWERTY_M,
	[0x33] = QWERTY_COMMA,
	[0x34] = QWERTY_PERIOD,
	[0x35] = QWERTY_FSLASH,
	[0x36] = QWERTY_RSHIFT,
	[0x37] = QWERTY_PAD_STAR,
	[0x38] = QWERTY_LALT,
	[0x39] = QWERTY_SPACE,
	[0x3A] = QWERTY_CAPS,
	[0x3B] = QWERTY_F1,
	[0x3C] = QWERTY_F2,
	[0x3D] = QWERTY_F3,
	[0x3E] = QWERTY_F4,
	[0x3F] = QWERTY_F5,
	[0x40] = QWERTY_F6,
	[0x41] = QWERTY_F7,
	[0x42] = QWERTY_F8,
	[0x43] = QWERTY_F9,
	[0x44] = QWERTY_F10,
	[0x45] = QWERTY_NUMLOCK,
	[0x46] = QWERTY_SCROLLLOCK,
	[0x47] = QWERTY_PAD_7,
	[0x48] = QWERTY_PAD_8,
	[0x49] = QWERTY_PAD_9,
	[0x4A] = QWERTY_PAD_MINUS,
	[0x4B] = QWERTY_PAD_4,
	[0x4C] = QWERTY_PAD_5,
	[0x4D] = QWERTY_PAD_6,
	[0x4E] = QWERTY_PAD_PLUS,
	[0x4F] = QWERTY_PAD_1,
	[0x50] = QWERTY_PAD_2,
	[0x51] = QWERTY_PAD_3,
	[0x52] = QWERTY_PAD_0,
	[0x53] = QWERTY_PAD_PERIOD,
	[0x57] = QWERTY_F11,
	[0x58] = QWERTY_F12
};

static const uint8_t keystate_scan_set_2_primary[0x90] = {
	[0x01] = QWERTY_F9,
	[0x03] = QWERTY_F5,
	[0x04] = QWERTY_F3,
	[0x05] = QWERTY_F1,
	[0x06] = QWERTY_F2,
	[0x07] = QWERTY_F12,
	[0x09] = QWERTY_F10,
	[0x0A] = QWERTY_F8,
	[0x0B] = QWERTY_F6,
	[0x0C] = QWERTY_F4,
	[0x0D] = QWERTY_TAB,
	[0x0E] = QWERTY_BACKTICK,
	[0x11] = QWERTY_LALT,
	[0x12] = QWERTY_LSHIFT,
	[0x14] = QWERTY_LCTRL,
	[0x15] = QWERTY_Q,
	[0x16] = QWERTY_1,
	[0x1A] = QWERTY_Z,
	[0x1B] = QWERTY_S,
	[0x1C] = QWERTY_A,
	[0x1D] = QWERTY_W,
	[0x1E] = QWERTY_2,
	[0x21] = QWERTY_C,
	[0x22] = QWERTY_X,
	[0x23] = QWERTY_D,
	[0x24] = QWERTY_E,
	[0x25] = QWERTY_4,
	[0x26] = QWERTY_3,
	[0x29] = QWERTY_SPACE,
	[0x2A] = QWERTY_V,
	[0x2B] = QWERTY_F,
	[0x2C] = QWERTY_T,
	[0x2D] = QWERTY_R,
	[0x2E] = QWERTY_5,
	[0x31] = QWERTY_N,
	[0x32] = QWERTY_B,
	[0x33] = QWERTY_H,
	[0x34] = QWERTY_G,
	[0x35] = QWERTY_Y,
	[0x36] = QWERTY_6,
	[0x3A] = QWERTY_M,
	[0x3B] = QWERTY_J,
	[0x3C] = QWERTY_U,
	[0x3D] = QWERTY_7,
	[0x3E] = QWERTY_8,
	[0x41] = QWERTY_COMMA,
	[0x42] = QWERTY_K,
	[0x44] = QWERTY_O,
	[0x45] = QWERTY_0,
	[0x46] = QWERTY_9,
	[0x49] = QWERTY_PERIOD,
	[0x4A] = QWERTY_FSLASH,
	[0x4B] = QWERTY_L,
	[0x4C] = QWERTY_SEMICOLON,
	[0x4D] = QWERTY_P,
	[0x4E] = QWERTY_MINUS,
	[0x52] = QWERTY_QUOTE,
	[0x54] = QWERTY_LBRACKET,
	[0x55] = QWERTY_EQUALS,
	[0x58] = QWERTY_CAPS,
	[0x59] = QWERTY_RSHIFT,
	[0x5A] = QWERTY_ENTER,
	[0x5B] = QWERTY_RBRACKET,
	[0x5D] = QWERTY_BSLASH,
	[0x66] = QWERTY_BACKSPACE,
	[0x69] = QWERTY_PAD_1,
	[0x6B] = QWERTY_PAD_4,
	[0x6C] = QWERTY_PAD_7,
	[0x70] = QWERTY_PAD_0,
	[0x71] = QWERTY_PAD_PERIOD,
	[0x72] = QWERTY_PAD_2,
	[0x73] = QWERTY_PAD_5,
	[0x74] = QWERTY_PAD_6,
	[0x75] = QWERTY_PAD_8,
	[0x76] = QWERTY_ESC,
	[0x77] = QWERTY_NUMLOCK,
	[0x78] = QWERTY_F11,
	[0x79] = QWERTY_PAD_PLUS,
	[0x7A] = QWERTY_PAD_3,
	[0x7B] = QWERTY_PAD_MINUS,
	[0x7C] = QWERTY_PAD_STAR,
	[0x7D] = QWERTY_PAD_9,
	[0x7E] = QWERTY_SCROLLLOCK,
	[0x83] = QWERTY_F7
};
