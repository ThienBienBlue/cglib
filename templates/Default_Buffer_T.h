/**
 * Contiguous fixed size heap allocated buffer with default element at index 0.
 * Owns all its memory. Call `free' to free the buffer.
 *
 * #include
 * - base.h
 */


/// Default value is at :buffer[0] to allow for returned indicies to be
/// if-checked.
struct Default_Buffer<T>
{
	u32 capacity;
	u32 length;
	T buffer[1];
};

/// :returns a Default_Buffer with memory for :capacity + (T){ 0 }.
function struct Default_Buffer<T>*
Default_Buffer<T>_init_zero(u32 capacity);

/// :returns a Default_Buffer with memory for :capacity + :default_.
function struct Default_Buffer<T>*
Default_Buffer<T>_init(u32 capacity, T default_);

/// Puts :item at :idx. A negative :idx will offset backwards from the end.
///
/// :returns index where :item was placed. 0 if not possible.
function u32
Default_Buffer<T>_put(struct Default_Buffer<T>* self, i32 idx, T item);

/// :returns index where :item was placed. 0 if not possible.
function u32
Default_Buffer<T>_push(struct Default_Buffer<T>* self, T item);

/// :returns the element popped.
function T
Default_Buffer<T>_pop(struct Default_Buffer<T>* self);
