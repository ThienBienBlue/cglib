#include <stdlib.h>
#include <string.h>

#include "../base.h"
#include "bindings.h"
#include "Buffer_Parametric_Binding.h"


function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(u32 capacity)
{
	struct Buffer_Parametric_Binding* retval = (struct Buffer_Parametric_Binding*)malloc(sizeof(struct Buffer_Parametric_Binding) + (capacity * sizeof(struct Parametric_Binding)));

	if (retval)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_copy_ptr(u32 capacity, struct Parametric_Binding* ptr)
{
	struct Buffer_Parametric_Binding* retval = Buffer_Parametric_Binding_init(capacity);

	if (retval)
	{
		memcpy(retval->buffer, ptr, sizeof(struct Parametric_Binding) * capacity);
		retval->length = capacity;
	}

	return retval;
}

function bool Buffer_Parametric_Binding_put(struct Buffer_Parametric_Binding* self, u32 idx, struct Parametric_Binding item)
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

function bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item)
{
	return Buffer_Parametric_Binding_put(self, self->length, item);
}

function bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self)
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

function bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, u32 left_idx, u32 right_idx)
{
	u32 len = self->length;

	if (left_idx < len && right_idx < len)
	{
		struct Parametric_Binding* buffer = self->buffer;

		struct Parametric_Binding temp = buffer[left_idx];
		buffer[left_idx] = buffer[right_idx];
		buffer[right_idx] = temp;

		return true;
	}
	else
	{
		return false;
	}
}
