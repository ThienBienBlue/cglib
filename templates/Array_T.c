#include <stdlib.h>

#include "../base.h"
#include "Array_<T>.h"


struct Array<T>* Array<T>_init(i32 capacity)
{
	struct Array<T> on_stack = Array<T>_sinit(capacity);
	struct Array<T>* on_heap = malloc(sizeof(struct Array<T>));

	if (on_heap != null)
	{
		*on_heap = on_stack;
	}

	return on_heap;
}

struct Array<T> Array<T>_sinit(i32 capacity)
{
	i32 actual_capacity = 0b1000;
	while (actual_capacity < capacity)
	{
		actual_capacity = actual_capacity << 1;
	}

	T* array = (T*)calloc(actual_capacity, sizeof(T));
	if (array == null)
	{
		actual_capacity = 0;
	}

	struct Array<T> retval = {
		.capacity = actual_capacity,
		.length = 0,
		.array = array
	};

	return retval;
}

void Array<T>_free(struct Array<T>* self)
{
	if (self != null)
	{
		free(self->array);
	}

	free(self);
}

void Array<T>_sfree(struct Array<T> self)
{
	free(self.array);
}

bool Array<T>_push(struct Array<T>* self, T item)
{
	if (self == null)
	{
		return false;
	}

	if (self->capacity <= self->length)
	{
		i32 capacity_new = 2 * self->capacity;
		T* array_new = (T*)reallocarray(self->array, capacity_new, sizeof(T));

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

bool Array<T>_pop(struct Array<T>* self)
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
