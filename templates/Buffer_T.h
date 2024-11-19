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
	i32 capacity;
	i32 length;
	T buffer[0];
};

struct Buffer<T>* Buffer<T>_init(i32 capacity);

struct Buffer<T>* Buffer<T>_from_ptr(i32 capacity, T* ptr);

#define Buffer<T>_from_carray(a) Buffer<T>_from_ptr(arraycount(a), (a))

struct Buffer<T>* Buffer<T>_filter(struct Buffer<T>* self, bool (*filter)(T));

bool Buffer<T>_put(struct Buffer<T>* self, i32 idx, T item);

bool Buffer<T>_push(struct Buffer<T>* self, T item);

bool Buffer<T>_pop(struct Buffer<T>* self);

bool Buffer<T>_swap(struct Buffer<T>* self, i32 left_idx, i32 right_idx);
