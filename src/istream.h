#ifndef ISTREAM_H
#define ISTREAM_H

#include <stdint.h>
#include <stddef.h>

#ifndef EOF
#define EOF 4
#endif

char istream_putc(char val);

char istream_getc();

char istream_putc_nonblk(char val);

char istream_getc_nonblk();

size_t istream_gets(char* buf, size_t num);

#endif
