#ifndef BUFFER_<T>_H_
#define BUFFER_<T>_H_

/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>
#include <stdlib.h>

struct Buffer_<T>
{
	int capacity;
	int length;
	T buffer[0];
};

struct Buffer_<T>* Buffer_<T>_init(int capacity)
{
	if (capacity < 0)
		capacity = 0;
	struct Buffer_<T>* retval = (struct Buffer_<T>*)malloc(sizeof(struct Buffer_<T>) + capacity * sizeof(T));
	retval->capacity = capacity;
	retval->length = 0;
	return retval;
}

bool Buffer_<T>_push(struct Buffer_<T>* self, T item)
{
	if (self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer_<T>_pop(struct Buffer_<T>* self)
{
	if (self->length <= 0)
		return false;
	self->length--;
	return true;
}

bool Buffer_<T>_swap(struct Buffer_<T>* self, int left_idx, int right_idx)
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
