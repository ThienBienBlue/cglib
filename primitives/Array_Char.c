#include <stdbool.h>
#include <stdlib.h>

#include "./Array_Char.h"

struct Array_Char* Array_Char_init(int capacity)
{
	int actual_capacity = 0b1000;
	while (actual_capacity < capacity)
		actual_capacity = actual_capacity << 1;

	char* array = (char*)calloc(sizeof(char), actual_capacity);
	if (array == NULL)
		return NULL;

	struct Array_Char* retval = (struct Array_Char*)malloc(sizeof(struct Array_Char));
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

void Array_Char_free(struct Array_Char* self)
{
	if (self == NULL)
		return;
	free(self->array);
	free(self);
}

bool Array_Char_push(struct Array_Char* self, char item)
{
	if (self == NULL)
		return false;

	if (self->capacity <= self->length)
	{
		int capacity_new = 2 * self->capacity;
		char* array_new = (char*)reallocarray(self->array, sizeof(char), capacity_new);
		if (array_new == NULL)
			return false;

		self->array = array_new;
		self->capacity = capacity_new;
	}

	self->array[self->length++] = item;
	return true;
}

bool Array_Char_concat(struct Array_Char* self, struct Array_Char* with)
{
	if (self == NULL)
		return false;

	int original_length = self->length;
	for (int idx = 0; idx < with->length; idx++)
	{
		if (!Array_Char_push(self, with->array[idx]))
		{
			self->length = original_length;
			return false;
		}
	}
	return true;
}

bool Array_Char_pop_many(struct Array_Char* self, int how_many)
{
	if (self == NULL || 0 <= self->length - how_many)
	{
		self->length = self->length - how_many;
		return true;
	}
	else
		return false;
}

bool Array_Char_pop(struct Array_Char* self)
{
	return Array_Char_pop_many(self, 1);
}

void Array_Char_concat_cstring(struct Array_Char *self, char *cstring, int cstring_length)
{
	struct Array_Char dummy;
	dummy.array = cstring;
	dummy.length = cstring_length;
	Array_Char_concat(self, &dummy);
}
