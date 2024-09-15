/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>

struct Buffer<T>
{
	i32 capacity;
	i32 length;
	T buffer[0];
};

struct Buffer<T>* Buffer<T>_init(i32 capacity);

struct Buffer<T>* Buffer<T>_filter(struct Buffer<T>* self, bool (*filter)(T));

bool Buffer<T>_put(struct Buffer<T>* self, i32 idx, T item);

bool Buffer<T>_push(struct Buffer<T>* self, T item);

bool Buffer<T>_pop(struct Buffer<T>* self);

bool Buffer<T>_swap(struct Buffer<T>* self, i32 left_idx, i32 right_idx);
