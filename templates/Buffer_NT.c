#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "./Buffer_<N,T>.h"

static bool write_to(struct Buffer<NT>* self, int idx, T item)
{
	T* write = self->buffer + idx;
	void* written = memcpy(write, &item, sizeof(T));

	return written != NULL;
}

struct Buffer<NT> Buffer<NT>_init(T zero)
{
	struct Buffer<NT> retval = {
		.length = 0
	};

	for (int i = 0; i < N; i++)
	{
		write_to(&retval, i, zero);
	}

	return retval;
}

bool Buffer<NT>_put(struct Buffer<NT>* self, int idx, T item)
{
	if (self != NULL && 0 <= idx && idx < N)
	{
		bool written = write_to(self, idx, item);

		if (written && self->length < idx)
		{
			self->length = idx;
		}

		return written;
	}
	else
	{
		return false;
	}
}

bool Buffer<NT>_push(struct Buffer<NT>* self, T item)
{
	if (self != NULL && self->length < N)
	{
		bool written = write_to(self, self->length, item);

		if (written)
		{
			self->length++;
		}

		return written;
	}
	else
	{
		return false;
	}
}

bool Buffer<NT>_pop(struct Buffer<NT>* self)
{
	if (self != NULL && 0 < self->length)
	{
		self->length--;

		return true;
	}
	else
	{
		return false;
	}
}
