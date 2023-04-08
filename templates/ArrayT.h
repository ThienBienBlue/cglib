/**
 * This file was automatically generated to be a generic Array.
 */

#include <stdbool.h>
#include <stdlib.h>

struct Array<T>
{
	int capacity;
	int length;
	T buffer[0];
};

struct Array<T>* Array<T>_init(int capacity)
{
	if (capacity < 16)
		capacity = 16;
	struct Array<T>* retval = (struct Array<T>*)malloc(sizeof(struct Array<T>) + capacity * sizeof(T));
	retval->capacity = capacity;
	retval->length = 0;
	return retval;
}

bool Array<T>_push(struct Array<T>* self, T item)
{
	if (self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Array<T>_pop(struct Array<T>* self)
{
	if (self->length <= 0)
		return false;
	self->length--;
	return true;
}

