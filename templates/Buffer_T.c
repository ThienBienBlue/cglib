#include <stdbool.h>
#include <stdlib.h>

#include "./Buffer_<T>.h"

struct Buffer<T>* Buffer<T>_init(int capacity)
{
	if (capacity < 0)
	{
		capacity = 0;
	}

	struct Buffer<T>* retval = (struct Buffer<T>*)malloc(sizeof(struct Buffer<T>) + capacity * sizeof(T));

	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

struct Buffer<T>* Buffer<T>_filter(struct Buffer<T>* self, bool (*filter)(T))
{
	int capacity = self->capacity;
	T* buffer = self->buffer;
	struct Buffer<T>* retval = Buffer<T>_init(capacity);

	for (int idx = 0; idx < capacity; idx++)
	{
		T item = buffer[idx];

		if (filter(item))
		{
			Buffer<T>_push(retval, item);
		}
	}

	return retval;
}

bool Buffer<T>_push(struct Buffer<T>* self, T item)
{
	if (self == NULL || self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer<T>_pop(struct Buffer<T>* self)
{
	if (self == NULL || self->length <= 0)
	{
		return false;
	}

	self->length--;

	return true;
}

bool Buffer<T>_swap(struct Buffer<T>* self, int left_idx, int right_idx)
{
	if (self == NULL)
	{
		return false;
	}

	int buffer_length = self->length;
	T* buffer = self->buffer;

	if (!(0 <= left_idx && left_idx < buffer_length && 0 <= right_idx && right_idx < buffer_length))
	{
		return false;
	}

	T temp = buffer[left_idx];
	buffer[left_idx] = buffer[right_idx];
	buffer[right_idx] = temp;

	return true;
}
