#include <stdlib.h>
#include <string.h>

#include "../base.h"
#include "Buffer_String.h"


function struct Buffer_String* Buffer_String_init(u32 capacity)
{
	struct Buffer_String* retval = (struct Buffer_String*)malloc(sizeof(struct Buffer_String) + (capacity * sizeof(struct String)));

	if (retval)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

function struct Buffer_String* Buffer_String_copy_ptr(u32 capacity, struct String* ptr)
{
	struct Buffer_String* retval = Buffer_String_init(capacity);

	if (retval)
	{
		memcpy(retval->buffer, ptr, sizeof(struct String) * capacity);
		retval->length = capacity;
	}

	return retval;
}

function bool Buffer_String_put(struct Buffer_String* self, u32 idx, struct String item)
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

function bool Buffer_String_push(struct Buffer_String* self, struct String item)
{
	return Buffer_String_put(self, self->length, item);
}

function bool Buffer_String_pop(struct Buffer_String* self)
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

function bool Buffer_String_swap(struct Buffer_String* self, u32 left_idx, u32 right_idx)
{
	u32 len = self->length;

	if (left_idx < len && right_idx < len)
	{
		struct String* buffer = self->buffer;

		struct String temp = buffer[left_idx];
		buffer[left_idx] = buffer[right_idx];
		buffer[right_idx] = temp;

		return true;
	}
	else
	{
		return false;
	}
}
