#include "istream.h"
#include "debug.h"
#include "queue.h"

#define ISTREAM_BUFFER_SIZE 1024

static char istream_buffer[ISTREAM_BUFFER_SIZE];
static volatile queue_t istream_queue = QUEUE((void*)istream_buffer, ISTREAM_BUFFER_SIZE, 1);

char istream_putc(char val)
{
	while(queue_is_full(&istream_queue))
		continue;
	
	queue_push(&istream_queue, (void*)&val);
	
	return val;
}

char istream_getc()
{
	while(queue_is_empty(&istream_queue))
		continue;
	
	char c;
	queue_pop(&istream_queue, &c);
	return c;
}

char istream_putc_nonblk(char val)
{
	if(queue_is_full(&istream_queue))
		return EOF;
	
	queue_push(&istream_queue, (void*)&val);
	
	return val;
}

char istream_getc_nonblk()
{
	if(queue_is_empty(&istream_queue))
		return EOF;
	
	char c;
	queue_pop(&istream_queue, &c);
	return c;
}

size_t istream_gets(char* buf, size_t num)
{
	if(num == 0)
		return 0;
	
	char* ptr = buf;
	while(1) {
		
		num--;
		if(!num)
			break;
		
		char c = istream_getc();
		
		if(c == '\n')
			break;
		
		*ptr++ = c;
	}
	
	*ptr = '\0';
	return ptr - buf;
}
