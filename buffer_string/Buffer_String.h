/**
 * Buffers are C-arrays that bundle the :capacity and :length.
 * All memory is owned by the buffer. Freeing memory is freeing the buffer.
 *
 * #include
 * - stdbool.h
 * - base.h
 */


struct Buffer_String
{
	i32 capacity;
	i32 length;
	struct String buffer[0];
};

struct Buffer_String* Buffer_String_init(i32 capacity);

struct Buffer_String* Buffer_String_from_ptr(i32 capacity, struct String* ptr);

#define Buffer_String_from_carray(a) Buffer_String_from_ptr(arraycount(a), (a))

struct Buffer_String* Buffer_String_filter(struct Buffer_String* self, bool (*filter)(struct String));

bool Buffer_String_put(struct Buffer_String* self, i32 idx, struct String item);

bool Buffer_String_push(struct Buffer_String* self, struct String item);

bool Buffer_String_pop(struct Buffer_String* self);

bool Buffer_String_swap(struct Buffer_String* self, i32 left_idx, i32 right_idx);
