/**
 * This file was automatically generated to be a generic Buffer.
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

