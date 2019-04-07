#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct queue {
	void* buffer;
	size_t size;
	size_t head;
	size_t tail;
	unsigned char width;
};
typedef struct queue queue_t;

inline bool queue_is_empty(queue_t* q)
{
	return q->tail == q->head;
}

inline bool queue_is_full(queue_t* q)
{
	return (q->tail + 1) % q->size == q->head;
}

inline void* queue_top(queue_t* q, void* buf)
{
	uint8_t* ptr = (uint8_t*)(q->buffer + q->head * q->width);
	uint8_t* end = ptr + q->width;
	uint8_t* buf_ptr = (uint8_t*)buf;
	while(ptr < end)
	{
		*buf_ptr++ = *ptr++;
	}
	return buf;
}

inline size_t queue_count(queue_t* q)
{
	return q->tail >= q->head ? q->tail - q->head : q->size + q->tail - q->head;
}

inline void* queue_pop(queue_t* q, void* buf)
{
	queue_top(q, buf);
	q->head = (q->head + 1) % q->size;
	
	return buf;
}

inline void queue_pip(queue_t* q)
{
	q->head = (q->head + 1) % q->size;
}

inline void queue_push(queue_t* q, void* buf)
{
	uint8_t* ptr = (uint8_t*)(q->buffer + q->tail * q->width);
	uint8_t* end = ptr + q->width;
	uint8_t* buf_ptr = (uint8_t*)buf;
	while(ptr < end)
	{
		*ptr++ = *buf_ptr++;
	}
	
	q->tail = (q->tail + 1) % q->size;
}

#define QUEUE(buffer, size, width) { buffer, size, 0, 0, width }

#endif
