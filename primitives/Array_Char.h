#ifndef ARRAY_Char_H_
#define ARRAY_Char_H_

/**
 * Generated code for a generic Array. Arrays dynamically resize themselves as needed.
 */
#include <stdbool.h>

struct Array_Char
{
	int capacity;
	int length;
	char* array;
};

struct Array_Char* Array_Char_init(int capacity);

void Array_Char_free(struct Array_Char* self);

bool Array_Char_push(struct Array_Char* self, char item);

bool Array_Char_concat(struct Array_Char* self, struct Array_Char* with);

bool Array_Char_pop_many(struct Array_Char* self, int how_many);

bool Array_Char_pop(struct Array_Char* self);

void Array_Char_concat_cstring(struct Array_Char* self, char* cstring, int cstring_length);

#endif
