/**
 * Buffers are C-arrays that bundle the :capacity and :length.
 * All memory is owned by the buffer is contiguous. Freeing the buffer frees
 * all memory owned by the buffer.
 *
 * #include
 * - base.h
 */


struct Buffer_String
{
	u32 capacity;
	u32 length;
	struct String buffer[0];
};

function struct Buffer_String* Buffer_String_init(u32 capacity);

function struct Buffer_String* Buffer_String_copy_ptr(u32 capacity, struct String* ptr);

#define Buffer_String_copy_carray(a) Buffer_String_copy_ptr(arraylen(a), (a))

function struct String Buffer_String_at(struct Buffer_String const* self, i32);

function bool Buffer_String_put(struct Buffer_String* self, u32 idx, struct String item);

function bool Buffer_String_push(struct Buffer_String* self, struct String item);

function bool Buffer_String_pop(struct Buffer_String* self);

function bool Buffer_String_swap(struct Buffer_String* self, u32 left_idx, u32 right_idx);
