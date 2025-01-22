#include <stdlib.h>
#include <string.h>

#include "../base.h"
#include "bindings.h"
#include "Buffer_Parametric_Binding.h"


function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(i32 capacity)
{
	capacity = max(0, capacity);

	struct Buffer_Parametric_Binding* retval = (struct Buffer_Parametric_Binding*)malloc(sizeof(struct Buffer_Parametric_Binding) + capacity * sizeof(struct Parametric_Binding));

	if (retval != null)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_from_ptr(i32 capacity, struct Parametric_Binding* ptr)
{
	struct Buffer_Parametric_Binding* retval = Buffer_Parametric_Binding_init(capacity);

	if (retval != null)
	{
		memcpy(retval->buffer, ptr, sizeof(struct Parametric_Binding) * capacity);
		retval->length = capacity;
	}

	return retval;
}

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_filter(struct Buffer_Parametric_Binding* self, bool (*filter)(struct Parametric_Binding))
{
	i32 capacity = self->capacity;
	struct Parametric_Binding* buffer = self->buffer;
	struct Buffer_Parametric_Binding* retval = Buffer_Parametric_Binding_init(capacity);

	for (i32 idx = 0; idx < capacity; idx++)
	{
		struct Parametric_Binding item = buffer[idx];

		if (filter(item))
		{
			Buffer_Parametric_Binding_push(retval, item);
		}
	}

	return retval;
}

function bool Buffer_Parametric_Binding_put(struct Buffer_Parametric_Binding* self, i32 idx, struct Parametric_Binding item)
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

function bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item)
{
	if (self != null)
	{
		return Buffer_Parametric_Binding_put(self, self->length, item);
	}
	else
	{
		return false;
	}
}

function bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self)
{
	if (self == null || self->length <= 0)
	{
		return false;
	}

	self->length--;

	return true;
}

function bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, i32 left_idx, i32 right_idx)
{
	if (self == null)
	{
		return false;
	}

	i32 len = self->length;
	struct Parametric_Binding* buffer = self->buffer;

	if (left_idx < 0 || len <= left_idx || right_idx < 0 || len <= right_idx)
	{
		return false;
	}

	struct Parametric_Binding left = buffer[left_idx];
	struct Parametric_Binding right = buffer[right_idx];

	bool left_written = Buffer_Parametric_Binding_put(self, right_idx, left);
	bool right_written = Buffer_Parametric_Binding_put(self, left_idx, right);

	return left_written && right_written;
}
