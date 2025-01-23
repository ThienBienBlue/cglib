#include <stdlib.h>
#include <string.h>

#include "Buffer_<T>.h"


function struct Buffer<T>* Buffer<T>_init(u32 capacity)
{
	struct Buffer<T>* retval = (struct Buffer<T>*)malloc(sizeof(struct Buffer<T>) + (capacity * sizeof(T)));

	if (retval)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

function struct Buffer<T>* Buffer<T>_copy_ptr(u32 capacity, T* ptr)
{
	struct Buffer<T>* retval = Buffer<T>_init(capacity);

	if (retval)
	{
		memcpy(retval->buffer, ptr, sizeof(T) * capacity);
		retval->length = capacity;
	}

	return retval;
}

function bool Buffer<T>_put(struct Buffer<T>* self, u32 idx, T item)
{
	if (idx < self->capacity)
	{
		self->buffer[idx] = item;

		if (self->length <= idx)
		{
			self->length = idx + 1;
		}

		return true;
	}
	else
	{
		return false;
	}
}

function bool Buffer<T>_push(struct Buffer<T>* self, T item)
{
	return Buffer<T>_put(self, self->length, item);
}

function bool Buffer<T>_pop(struct Buffer<T>* self)
{
	if (0 < self->length)
	{
		self->length--;

		return true;
	}
	else
	{
		return false;
	}
}

function bool Buffer<T>_swap(struct Buffer<T>* self, u32 left_idx, u32 right_idx)
{
	u32 len = self->length;

	if (left_idx < len && right_idx < len)
	{
		T* buffer = self->buffer;

		T temp = buffer[left_idx];
		buffer[left_idx] = buffer[right_idx];
		buffer[right_idx] = temp;

		return true;
	}
	else
	{
		return false;
	}
}
