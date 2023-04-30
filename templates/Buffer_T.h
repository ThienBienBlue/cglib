#ifndef BUFFER_<T>_H_
#define BUFFER_<T>_H_

/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>

struct Buffer_<T>
{
	int capacity;
	int length;
	T buffer[0];
};

struct Buffer_<T>* Buffer_<T>_init(int capacity);

bool Buffer_<T>_push(struct Buffer_<T>* self, T item);

bool Buffer_<T>_pop(struct Buffer_<T>* self);

bool Buffer_<T>_swap(struct Buffer_<T>* self, int left_idx, int right_idx);

#endif  // BUFFER_<T>_H_
