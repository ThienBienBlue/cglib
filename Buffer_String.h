/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>

struct Buffer_String
{
	i32 capacity;
	i32 length;
	struct String buffer[0];
};

struct Buffer_String* Buffer_String_init(i32 capacity);

struct Buffer_String* Buffer_String_filter(struct Buffer_String* self, bool (*filter)(struct String));

bool Buffer_String_put(struct Buffer_String* self, i32 idx, struct String item);

bool Buffer_String_push(struct Buffer_String* self, struct String item);

bool Buffer_String_pop(struct Buffer_String* self);

bool Buffer_String_swap(struct Buffer_String* self, i32 left_idx, i32 right_idx);
