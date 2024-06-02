#ifndef BUFFER<T>_H_
#define BUFFER<T>_H_

/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>

struct Buffer<T>
{
	int capacity;
	int length;
	T buffer[0];
};

struct Buffer<T>* Buffer<T>_init(int capacity);

struct Buffer<T>* Buffer<T>_filter(struct Buffer<T>* self, bool (*filter)(T));

bool Buffer<T>_put(struct Buffer<T>* self, int idx, T item);

bool Buffer<T>_push(struct Buffer<T>* self, T item);

bool Buffer<T>_pop(struct Buffer<T>* self);

bool Buffer<T>_swap(struct Buffer<T>* self, int left_idx, int right_idx);

#endif  // BUFFER<T>_H_
