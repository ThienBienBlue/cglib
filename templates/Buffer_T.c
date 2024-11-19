#include <stdlib.h>
#include <string.h>

#include "./base.h"
#include "./Buffer_<T>.h"


struct Buffer<T>* Buffer<T>_init(i32 capacity)
{
	capacity = max(0, capacity);

	struct Buffer<T>* retval = (struct Buffer<T>*)malloc(sizeof(struct Buffer<T>) + capacity * sizeof(T));

	if (retval != null)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

struct Buffer<T>* Buffer<T>_from_ptr(i32 capacity, T* ptr)
{
	struct Buffer<T>* retval = Buffer<T>_init(capacity);

	if (retval != null)
	{
		memcpy(retval->buffer, ptr, sizeof(T) * capacity);
		retval->length = capacity;
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
	if (self != null && idx < self->capacity)
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

bool Buffer<T>_push(struct Buffer<T>* self, T item)
{
	if (self != null)
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
	if (self == null || self->length <= 0)
	{
		return false;
	}

	self->length--;

	return true;
}

bool Buffer<T>_swap(struct Buffer<T>* self, i32 left_idx, i32 right_idx)
{
	if (self == null)
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
