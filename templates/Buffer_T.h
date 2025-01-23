/**
 * Buffers are C-arrays that bundle the :capacity and :length.
 * All memory is owned by the buffer is contiguous. Freeing the buffer frees
 * all memory owned by the buffer.
 *
 * #include
 * - base.h
 */


struct Buffer<T>
{
	u32 capacity;
	u32 length;
	T buffer[0];
};

function struct Buffer<T>* Buffer<T>_init(u32 capacity);

function struct Buffer<T>* Buffer<T>_copy_ptr(u32 capacity, T* ptr);

#define Buffer<T>_copy_carray(a) Buffer<T>_copy_ptr(arraylen(a), (a))

function T Buffer<T>_at(struct Buffer<T> const* self, i32);

function bool Buffer<T>_put(struct Buffer<T>* self, u32 idx, T item);

function bool Buffer<T>_push(struct Buffer<T>* self, T item);

function bool Buffer<T>_pop(struct Buffer<T>* self);

function bool Buffer<T>_swap(struct Buffer<T>* self, u32 left_idx, u32 right_idx);
