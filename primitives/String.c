#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"

struct String EMPTY_STRING = {0};
struct String_Builder EMPTY_BUILDER = {0};

struct Arena Arena_init(int capacity)
{
	char* buf = (char*)malloc(capacity);

	if (buf != NULL)
	{
		return (struct Arena) {
			.offset = 0,
			.capacity = capacity,
			.bytes = buf
		};
	}
	else
	{
		return (struct Arena) {0};
	}
}

struct String String_empty()
{
	return EMPTY_STRING;
}

struct String String_wrap(char const* cstring)
{
	return (struct String) {
		.length = strlen(cstring),
		.str = cstring
	};
}

struct String_Builder String_Builder_init(struct Arena* arena, int capacity)
{
	bool fits = 0 < capacity && (arena->offset + capacity <= arena->capacity);

	if (fits || capacity < 0)
	{
		int cap = (fits) ? capacity : arena->capacity - arena->offset;
		struct String_Builder retval = {
			.length = 0,
			.capacity = cap,
			.str = arena->bytes + arena->offset
		};

		arena->offset += cap;

		return retval;
	}
	else
	{
		return (struct String_Builder) {0};
	}
}

struct String_Builder String_append(struct String_Builder s1, struct String s2)
{
	bool fits = s1.length + s2.length <= s1.capacity;

	if (fits)
	{
		char* copied = memmove(s1.str + s1.length, s2.str, s2.length);

		if (copied != NULL)
		{
			s1.length += s2.length;

			return s1;
		}
		else
		{
			return EMPTY_BUILDER;
		}
	}
	else
	{
		return EMPTY_BUILDER;
	}
}

struct String_Builder String_push(struct String_Builder s1, char c)
{
	if (0 < s1.capacity - s1.length)
	{
		s1.str[s1.length++] = c;

		return s1;
	}
	else
	{
		return EMPTY_BUILDER;
	}
}

struct String String_Builder_build(struct String_Builder s)
{
	return (struct String) {
		.length = s.length,
		.str = s.str
	};
}
