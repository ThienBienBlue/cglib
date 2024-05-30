#ifndef ARRAY<T>_H_
#define ARRAY<T>_H_

/**
 * Generated code for a generic Array. Arrays dynamically resize themselves as needed.
 */
#include <stdbool.h>

struct Array<T>
{
	int capacity;
	int length;
	T* array;
};

struct Array<T>* Array<T>_init(int capacity);

void Array<T>_free(struct Array<T>* self);

bool Array<T>_push(struct Array<T>* self, T item);

bool Array<T>_concat(struct Array<T>* self, struct Array<T>* with);

bool Array<T>_pop_many(struct Array<T>* self, int how_many);

bool Array<T>_pop(struct Array<T>* self);

#endif
