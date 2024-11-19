#include <stdlib.h>

#include "../base.h"
#include "Array_String.h"


struct Array_String* Array_String_init(i32 capacity)
{
	struct Array_String on_stack = Array_String_sinit(capacity);
	struct Array_String* on_heap = malloc(sizeof(struct Array_String));

	if (on_heap != null)
	{
		*on_heap = on_stack;
	}

	return on_heap;
}

struct Array_String Array_String_sinit(i32 capacity)
{
	i32 actual_capacity = 0b1000;
	while (actual_capacity < capacity)
	{
		actual_capacity = actual_capacity << 1;
	}

	struct String* array = (struct String*)calloc(actual_capacity, sizeof(struct String));
	if (array == null)
	{
		actual_capacity = 0;
	}

	struct Array_String retval = {
		.capacity = actual_capacity,
		.length = 0,
		.array = array
	};

	return retval;
}

void Array_String_free(struct Array_String* self)
{
	if (self != null)
	{
		free(self->array);
	}

	free(self);
}

void Array_String_sfree(struct Array_String self)
{
	free(self.array);
}

bool Array_String_push(struct Array_String* self, struct String item)
{
	if (self == null)
	{
		return false;
	}

	if (self->capacity <= self->length)
	{
		i32 capacity_new = 2 * self->capacity;
		struct String* array_new = (struct String*)reallocarray(self->array, capacity_new, sizeof(struct String));

		if (array_new == null)
		{
			return false;
		}

		self->array = array_new;
		self->capacity = capacity_new;
	}

	self->array[self->length++] = item;

	return true;
}

bool Array_String_pop(struct Array_String* self)
{
	if (self != null && 0 < self->length)
	{
		self->length--;

		return true;
	}
	else
	{
		return false;
	}
}
