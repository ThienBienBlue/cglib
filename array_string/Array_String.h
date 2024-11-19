/**
 * Generated code for a generic Array. Arrays dynamically resize themselves as needed.
 *
 * #include
 * - base.h
 */


struct Array_String
{
	i32 capacity;
	i32 length;
	struct String* array;
};

struct Array_String* Array_String_init(i32 capacity);

struct Array_String Array_String_sinit(i32 capacity);

void Array_String_free(struct Array_String* self);

void Array_String_sfree(struct Array_String self);

bool Array_String_push(struct Array_String* self, struct String item);

bool Array_String_pop(struct Array_String* self);
