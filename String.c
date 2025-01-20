#include <stdbool.h>
#include <string.h>

#include "base.h"
#include "Arena.h"
#include "String.h"


struct String_Builder String_Builder_init(struct Arena* arena, i32 capacity)
{
	struct Arena_Alloc buffer = Arena_pack(arena, capacity);

	if (!iszero(buffer))
	{
		return (struct String_Builder){
			capacity,
			0,
			arena->bytes + buffer.offset
		};
	}
	else
	{
		return (struct String_Builder){ 0 };
	}
}

struct String_Builder String_append(struct String_Builder s1, struct String s2)
{
	bool fits = s1.length + s2.length <= s1.capacity;

	if (fits)
	{
		char* copied = memmove(s1.str + s1.length, s2.str, s2.length);

		if (copied)
		{
			s1.length += s2.length;

			return s1;
		}
		else
		{
			return (struct String_Builder){ 0 };
		}
	}
	else
	{
		return (struct String_Builder){ 0 };
	}
}

struct String_Builder String_push(struct String_Builder s1, char c)
{
	if (s1.length < s1.capacity)
	{
		s1.str[s1.length++] = c;

		return s1;
	}
	else
	{
		return (struct String_Builder){ 0 };
	}
}

struct String String_Builder_build(struct String_Builder s)
{
	return (struct String){
		.length = s.length,
		.str = s.str
	};
}
