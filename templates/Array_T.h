#ifndef ARRAY_<T>_H_
#define ARRAY_<T>_H_

/**
 * Generated code for a generic Array. Arrays dynamically resize themselves as needed.
 */
#include <stdbool.h>
#include <stdlib.h>

struct Array_<T>
{
	int capacity;
	int length;
	T* array;
};

struct Array_<T>* Array_<T>_init(int capacity)
{
	int actual_capacity = 0b1000;
	while (actual_capacity < capacity)
		actual_capacity = actual_capacity << 1;

	T* array = (T*)calloc(sizeof(T), actual_capacity);
	struct Array_<T>* retval = (struct Array_<T>*)malloc(sizeof(struct Array_<T>));
	retval->capacity = actual_capacity;
	retval->length = 0;
	retval->array = array;

	return retval;
}

void Array_<T>_free(struct Array_<T>* self)
{
	if (self == NULL)
		return;
	free(self->array);
	free(self);
}

bool Array_<T>_push(struct Array_<T>* self, T item)
{
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

bool Array_<T>_concat(struct Array_<T>* self, struct Array_<T>* with)
{
	int original_length = self->length;
	for (int idx = 0; idx < with->length; idx++)
	{
		if (!Array_<T>_push(self, with->array[idx]))
		{
			self->length = original_length;
			return false;
		}
	}
	return true;
}

bool Array_<T>_pop(struct Array_<T>* self)
{
	if (0 < self->length)
	{
		self->length--;
		return true;
	}
	else
		return false;
}

bool Array_<T>_pop_many(struct Array_<T>* self, int how_many)
{
	if (0 <= self->length - how_many)
	{
		self->length = self->length - how_many;
		return true;
	}
	else
		return false;
}

#endif  // ARRAY_<T>_H_
