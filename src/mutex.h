#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>

inline void mutex_spinlock(uint8_t* mutex)
{
	while(!__sync_bool_compare_and_swap(mutex, 0, 1))
		continue;
}

inline uint8_t mutex_trylock(uint8_t* mutex)
{
	return __sync_bool_compare_and_swap(mutex, 0, 1);
}

inline void mutex_unlock(uint8_t* mutex)
{
	*mutex = 0;
}

#endif
