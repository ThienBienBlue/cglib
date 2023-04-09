#ifndef BUFFER_<T>_H_
#define BUFFER_<T>_H_

/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>
#include <stdlib.h>

struct Buffer<T>
{
	int capacity;
	int length;
	T buffer[0];
};

struct Buffer<T>* Buffer<T>_init(int capacity)
{
	if (capacity < 0)
		capacity = 0;
	struct Buffer<T>* retval = (struct Buffer<T>*)malloc(sizeof(struct Buffer<T>) + capacity * sizeof(T));
	retval->capacity = capacity;
	retval->length = 0;
	return retval;
}

bool Buffer<T>_push(struct Buffer<T>* self, T item)
{
	if (self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer<T>_pop(struct Buffer<T>* self)
{
	if (self->length <= 0)
		return false;
	self->length--;
	return true;
}

bool Buffer<T>_swap(struct Buffer<T>* self, int left_idx, int right_idx)
{
	int buffer_length = self->length;
	if (!(0 <= left_idx && left_idx < buffer_length && 0 <= right_idx && right_idx < buffer_length))
		return false;
	T* buffer = self->buffer;
	T temp = buffer[left_idx];
	buffer[left_idx] = buffer[right_idx];
	buffer[right_idx] = temp;
	return true;
}

#endif  // BUFFER_<T>_H_
