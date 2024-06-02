#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "./String.h"
#include "./Parametric_Binding.h"
#include "./Buffer_Parametric_Binding.h"

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(int capacity)
{
	if (capacity < 0)
	{
		capacity = 0;
	}

	struct Buffer_Parametric_Binding* retval = (struct Buffer_Parametric_Binding*)malloc(sizeof(struct Buffer_Parametric_Binding) + capacity * sizeof(struct Parametric_Binding));

	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_filter(struct Buffer_Parametric_Binding* self, bool (*filter)(struct Parametric_Binding))
{
	int capacity = self->capacity;
	struct Parametric_Binding* buffer = self->buffer;
	struct Buffer_Parametric_Binding* retval = Buffer_Parametric_Binding_init(capacity);

	for (int idx = 0; idx < capacity; idx++)
	{
		struct Parametric_Binding item = buffer[idx];

		if (filter(item))
		{
			Buffer_Parametric_Binding_push(retval, item);
		}
	}

	return retval;
}

bool Buffer_Parametric_Binding_put(struct Buffer_Parametric_Binding* self, int idx, struct Parametric_Binding item)
{
	if (self != NULL && idx < self->capacity)
	{
		// Assignment won't compile if struct Parametric_Binding has const fields.
		struct Parametric_Binding* write = self->buffer + idx;
		void* written = memmove(write, &item, sizeof(struct Parametric_Binding));

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

bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item)
{
	if (self != NULL)
	{
		return Buffer_Parametric_Binding_put(self, self->length, item);
	}
	else
	{
		return false;
	}
}

bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self)
{
	if (self == NULL || self->length <= 0)
	{
		return false;
	}

	self->length--;

	return true;
}

bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, int left_idx, int right_idx)
{
	if (self == NULL)
	{
		return false;
	}

	int len = self->length;
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

struct Parametric_Binding const* Buffer_Parametric_Binding_find(
		struct Buffer_Parametric_Binding const *binding, char parametric)
{
	for (int idx = 0; idx < binding->length; idx++)
	{
		char matching_parametric = binding->buffer[idx].parametric;

		if (parametric == matching_parametric)
		{
			return binding->buffer + idx;
		}
	}

	return NULL;
}
