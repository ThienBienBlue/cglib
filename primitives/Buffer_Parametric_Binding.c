#include <stdbool.h>
#include <stdlib.h>

#include "./Buffer_Parametric_Binding.h"

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(int capacity)
{
	if (capacity < 0)
		capacity = 0;
	struct Buffer_Parametric_Binding* retval = (struct Buffer_Parametric_Binding*)malloc(sizeof(struct Buffer_Parametric_Binding) + capacity * sizeof(struct Parametric_Binding));
	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item)
{
	if (self == NULL || self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self)
{
	if (self == NULL || self->length <= 0)
		return false;
	self->length--;
	return true;
}

bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, int left_idx, int right_idx)
{
	if (self == NULL)
		return false;

	int buffer_length = self->length;
	struct Parametric_Binding* buffer = self->buffer;
	if (!(0 <= left_idx && left_idx < buffer_length && 0 <= right_idx && right_idx < buffer_length))
		return false;

	struct Parametric_Binding temp = buffer[left_idx];
	buffer[left_idx] = buffer[right_idx];
	buffer[right_idx] = temp;

	return true;
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
