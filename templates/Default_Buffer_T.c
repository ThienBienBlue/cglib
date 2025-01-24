#include <stdlib.h>

#include "Default_Buffer<T>.h"


function struct Default_Buffer<T>*
Default_Buffer<T>_init_zero(u32 capacity)
{
	return Default_Buffer<T>_init(capacity, (T){ 0 });
}

function struct Default_Buffer<T>*
Default_Buffer<T>_init(u32 capacity, T default_)
{
	u32 capacity_with_default = capacity + 1;
	struct Default_Buffer<T>* retval = malloc(sizeof(struct Default_Buffer<T>)
			+ (capacity_with_default * sizeof(T)));

	if (retval)
	{
		retval->capacity = capacity_with_default;
		retval->length = 1;
		retval->buffer[0] = default_;
	}

	return retval;
}

function u32
Default_Buffer<T>_put(struct Default_Buffer<T>* self, i32 idx, T item)
{
	i32 idx_positive = (idx < 0) ? idx + (i32)self->capacity : idx;
	u32 idx_ = idx_positive;  // Here for u32 i32 comparison.

	if (0 < idx_positive && idx_ < self->capacity)
	{
		self->buffer[idx_] = item;
		self->length = max(self->length, idx_ + 1);

		return idx_;
	}
	else
	{
		return 0;
	}
}

function u32
Default_Buffer<T>_push(struct Default_Buffer<T>* self, T item)
{
	return Default_Buffer<T>_put(self, self->length, item);
}

function T
Default_Buffer<T>_pop(struct Default_Buffer<T>* self)
{
	T retval = self->buffer[self->length - 1];

	if (1 < self->length)
	{
		self->length--;
	}

	return retval;
}
