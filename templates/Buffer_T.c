#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

bool Buffer<T>_write_to(struct Buffer<T>* self, T item, int idx)
{
	// Assignment won't compile if T has const fields.
	T* write = self->buffer + idx;
	void* written = memmove(write, &item, sizeof(T));

	return written != NULL;
}

bool Buffer<T>_push(struct Buffer<T>* self, T item)
{

	if (self != NULL && self->length < self->capacity)
	{
		bool retval = write_to(self, item, self->length);

		if (retval)
		{
			self->length++;
		}

		return retval;
	}
	else
	{
		return false;
	}
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

	int len = self->length;
	T* buffer = self->buffer;

	if (left_idx < 0 || len <= left_idx || right_idx < 0 || len <= right_idx)
	{
		return false;
	}

	T left = buffer[left_idx];
	T right = buffer[right_idx];

	bool left_written = write_to(self, right, left_idx);
	bool right_written = write_to(self, left, right_idx);

	return left_written && right_written;
}
