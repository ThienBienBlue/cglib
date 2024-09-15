#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "./base.h"
#include "./Buffer_<T>.h"

struct Buffer<T>* Buffer<T>_init(i32 capacity)
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
	i32 capacity = self->capacity;
	T* buffer = self->buffer;
	struct Buffer<T>* retval = Buffer<T>_init(capacity);

	for (i32 idx = 0; idx < capacity; idx++)
	{
		T item = buffer[idx];

		if (filter(item))
		{
			Buffer<T>_push(retval, item);
		}
	}

	return retval;
}

bool Buffer<T>_put(struct Buffer<T>* self, i32 idx, T item)
{
	if (self != NULL && idx < self->capacity)
	{
		// Assignment won't compile if T has const fields.
		T* write = self->buffer + idx;
		void* written = memmove(write, &item, sizeof(T));

		if (written != NULL && self->length <= idx)
		{
			self->length = idx + 1;
		}

		return written != NULL;
	}
	else
	{
		return false;
	}
}

bool Buffer<T>_push(struct Buffer<T>* self, T item)
{
	if (self != NULL)
	{
		return Buffer<T>_put(self, self->length, item);
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

bool Buffer<T>_swap(struct Buffer<T>* self, i32 left_idx, i32 right_idx)
{
	if (self == NULL)
	{
		return false;
	}

	i32 len = self->length;
	T* buffer = self->buffer;

	if (left_idx < 0 || len <= left_idx || right_idx < 0 || len <= right_idx)
	{
		return false;
	}

	T left = buffer[left_idx];
	T right = buffer[right_idx];

	bool left_written = Buffer<T>_put(self, right_idx, left);
	bool right_written = Buffer<T>_put(self, left_idx, right);

	return left_written && right_written;
}
