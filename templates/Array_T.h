/**
 * Generated code for a generic Array. Arrays dynamically resize themselves as needed.
 *
 * #include
 * - base.h
 */


struct Array<T>
{
	i32 capacity;
	i32 length;
	T* array;
};

struct Array<T>* Array<T>_init(i32 capacity);

struct Array<T> Array<T>_sinit(i32 capacity);

void Array<T>_free(struct Array<T>* self);

void Array<T>_sfree(struct Array<T> self);

bool Array<T>_push(struct Array<T>* self, T item);

bool Array<T>_pop(struct Array<T>* self);
