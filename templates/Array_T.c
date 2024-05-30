#include <stdbool.h>
#include <stdlib.h>

#include "./Array_<T>.h"

struct Array<T>* Array<T>_init(int capacity)
{
	int actual_capacity = 0b1000;
	while (actual_capacity < capacity)
		actual_capacity = actual_capacity << 1;

	T* array = (T*)calloc(sizeof(T), actual_capacity);
	if (array == NULL)
		return NULL;

	struct Array<T>* retval = (struct Array<T>*)malloc(sizeof(struct Array<T>));
	if (retval == NULL)
	{
		free(array);
		return NULL;
	}

	retval->capacity = actual_capacity;
	retval->length = 0;
	retval->array = array;

	return retval;
}

void Array<T>_free(struct Array<T>* self)
{
	if (self == NULL)
		return;
	free(self->array);
	free(self);
}

bool Array<T>_push(struct Array<T>* self, T item)
{
	if (self == NULL)
		return false;

	if (self->capacity <= self->length)
	{
		int capacity_new = 2 * self->capacity;
		T* array_new = (T*)reallocarray(self->array, sizeof(T), capacity_new);
		if (array_new == NULL)
			return false;

		self->array = array_new;
		self->capacity = capacity_new;
	}

	self->array[self->length++] = item;
	return true;
}

bool Array<T>_concat(struct Array<T>* self, struct Array<T>* with)
{
	if (self == NULL)
		return false;

	int original_length = self->length;
	for (int idx = 0; idx < with->length; idx++)
	{
		if (!Array<T>_push(self, with->array[idx]))
		{
			self->length = original_length;
			return false;
		}
	}
	return true;
}

bool Array<T>_pop_many(struct Array<T>* self, int how_many)
{
	if (self == NULL || 0 <= self->length - how_many)
	{
		self->length = self->length - how_many;
		return true;
	}
	else
		return false;
}

bool Array<T>_pop(struct Array<T>* self)
{
	return Array<T>_pop_many(self, 1);
}
