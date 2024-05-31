#include <stdbool.h>
#include <stdlib.h>

#include "./Buffer_String.h"

#include "./String.h"

struct Buffer_String* Buffer_String_init(int capacity)
{
	if (capacity < 0)
	{
		capacity = 0;
	}

	struct Buffer_String* retval = (struct Buffer_String*)malloc(sizeof(struct Buffer_String) + capacity * sizeof(struct String));

	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

struct Buffer_String* Buffer_String_filter(struct Buffer_String* self, bool (*filter)(struct String))
{
	int capacity = self->capacity;
	struct String* buffer = self->buffer;
	struct Buffer_String* retval = Buffer_String_init(capacity);

	for (int idx = 0; idx < capacity; idx++)
	{
		struct String item = buffer[idx];

		if (filter(item))
		{
			Buffer_String_push(retval, item);
		}
	}

	return retval;
}

bool Buffer_String_push(struct Buffer_String* self, struct String item)
{
	if (self == NULL || self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer_String_pop(struct Buffer_String* self)
{
	if (self == NULL || self->length <= 0)
	{
		return false;
	}

	self->length--;

	return true;
}

bool Buffer_String_swap(struct Buffer_String* self, int left_idx, int right_idx)
{
	if (self == NULL)
	{
		return false;
	}

	int buffer_length = self->length;
	struct String* buffer = self->buffer;

	if (!(0 <= left_idx && left_idx < buffer_length && 0 <= right_idx && right_idx < buffer_length))
	{
		return false;
	}

	struct String temp = buffer[left_idx];
	buffer[left_idx] = buffer[right_idx];
	buffer[right_idx] = temp;

	return true;
}
