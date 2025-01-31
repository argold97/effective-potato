#include "qwerty.h"

const struct keystate_entry querty_state_table[110] = {
	[QWERTY_1] = { 0, '1', '!' },
	[QWERTY_2] = { 0, '2', '@' },
	[QWERTY_3] = { 0, '3', '#' },
	[QWERTY_4] = { 0, '4', '$' },
	[QWERTY_5] = { 0, '5', '%' },
	[QWERTY_6] = { 0, '6', '^' },
	[QWERTY_7] = { 0, '7', '&' },
	[QWERTY_8] = { 0, '8', '*' },
	[QWERTY_9] = { 0, '9', '(' },
	[QWERTY_0] = { 0, '0', ')' },
	[QWERTY_MINUS] = { 0, '-', '_' },
	[QWERTY_EQUALS] = { 0, '=', '+' },
	[QWERTY_BACKSPACE] = { 0, 0x8, 0x8 },
	[QWERTY_TAB] = { 0, '\t', '\t' },
	[QWERTY_Q] = { 0, 'q', 'Q' },
	[QWERTY_W] = { 0, 'w', 'W' },
	[QWERTY_E] = { 0, 'e', 'E' },
	[QWERTY_R] = { 0, 'r', 'R' },
	[QWERTY_T] = { 0, 't', 'T' },
	[QWERTY_Y] = { 0, 'y', 'Y' },
	[QWERTY_U] = { 0, 'u', 'U' },
	[QWERTY_I] = { 0, 'i', 'I' },
	[QWERTY_O] = { 0, 'o', 'O' },
	[QWERTY_P] = { 0, 'p', 'P' },
	[QWERTY_LBRACKET] = { 0, '[', '{' },
	[QWERTY_RBRACKET] = { 0, ']', '}' },
	[QWERTY_ENTER] = { 0, '\n', '\n' },
	[QWERTY_A] = { 0, 'a', 'A' },
	[QWERTY_S] = { 0, 's', 'S' },
	[QWERTY_D] = { 0, 'd', 'D' },
	[QWERTY_F] = { 0, 'f', 'F' },
	[QWERTY_G] = { 0, 'g', 'G' },
	[QWERTY_H] = { 0, 'h', 'H' },
	[QWERTY_J] = { 0, 'j', 'J' },
	[QWERTY_K] = { 0, 'k', 'K' },
	[QWERTY_L] = { 0, 'l', 'L' },
	[QWERTY_SEMICOLON] = { 0, ';', ':' },
	[QWERTY_QUOTE] = { 0, '\'', '"' },
	[QWERTY_BACKTICK] = { 0, '`', '~' },
	[QWERTY_BSLASH] = { 0, '\\', '|' },
	[QWERTY_Z] = { 0, 'z', 'Z' },
	[QWERTY_X] = { 0, 'x', 'X' },
	[QWERTY_C] = { 0, 'c', 'C' },
	[QWERTY_V] = { 0, 'v', 'V' },
	[QWERTY_B] = { 0, 'b', 'B' },
	[QWERTY_N] = { 0, 'n', 'N' },
	[QWERTY_M] = { 0, 'm', 'M' },
	[QWERTY_COMMA] = { 0, ',', '<' },
	[QWERTY_PERIOD] = { 0, '.', '>' },
	[QWERTY_FSLASH] = { 0, '/', '?' },
	[QWERTY_PAD_STAR] = { 0, '*', '*' },
	[QWERTY_SPACE] = { 0, ' ', ' '},
	[QWERTY_PAD_7] = { 0, '7', '7' },
	[QWERTY_PAD_8] = { 0, '8', '8' },
	[QWERTY_PAD_9] = { 0, '9', '9' },
	[QWERTY_PAD_MINUS] = { 0, '-', '-' },
	[QWERTY_PAD_4] = { 0, '4', '4' },
	[QWERTY_PAD_5] = { 0, '5', '5' },
	[QWERTY_PAD_6] = { 0, '6', '6' },
	[QWERTY_PAD_PLUS] = { 0, '+', '+' },
	[QWERTY_PAD_1] = { 0, '1', '1' },
	[QWERTY_PAD_2] = { 0, '2', '2' },
	[QWERTY_PAD_3] = { 0, '3', '3' },
	[QWERTY_PAD_0] = { 0, '0', '0' },
	[QWERTY_PAD_PERIOD] = { 0, '.', '.' },
	[QWERTY_PAD_FSLASH] = { 0, '/', '/' },
	[QWERTY_PAD_ENTER] = { 0, '\n', '\n' }
};
