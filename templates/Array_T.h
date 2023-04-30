#ifndef ARRAY_<T>_H_
#define ARRAY_<T>_H_

/**
 * Generated code for a generic Array. Arrays dynamically resize themselves as needed.
 */
#include <stdbool.h>

struct Array_<T>
{
	int capacity;
	int length;
	T* array;
};

struct Array_<T>* Array_<T>_init(int capacity);

void Array_<T>_free(struct Array_<T>* self);

bool Array_<T>_push(struct Array_<T>* self, T item);

bool Array_<T>_concat(struct Array_<T>* self, struct Array_<T>* with);

bool Array_<T>_pop_many(struct Array_<T>* self, int how_many);

bool Array_<T>_pop(struct Array_<T>* self);

#endif
