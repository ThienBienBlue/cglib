#include <stdbool.h>
#include <string.h>

#include "String.h"

struct String EMPTY_STRING = {0};

struct String String_empty()
{
	return EMPTY_STRING;
}

struct String String_init(struct Arena* arena, int capacity)
{
	bool fits = 0 < capacity && (arena->offset + capacity <= arena->capacity);

	if (fits || capacity < 0)
	{
		int cap = (fits) ? capacity : arena->capacity - arena->offset;
		struct String retval;

		retval.length = 0;
		retval.capacity = cap;
		retval.str = arena->bytes + arena->offset;

		arena->offset += cap;

		return retval;
	}
	else
	{
		return EMPTY_STRING;
	}
}

struct String String_from_cstring(char* cstring)
{
	struct String retval;
	int length = strlen(cstring);

	retval.length = length;
	retval.capacity = length;
	retval.str = cstring;

	return retval;
}

struct String String_append(struct String s1, struct String s2)
{
	bool fits = s1.length + s2.length <= s1.capacity;

	if (fits)
	{
		struct String retval = s1;

		for (int idx = 0; idx < s2.length; idx++)
		{
			char c = s2.str[idx];

			retval.str[retval.length++] = c;
		}

		return retval;
	}
	else
	{
		return EMPTY_STRING;
	}
}

struct String String_push(struct String s1, char c)
{
	if (0 < s1.capacity - s1.length)
	{
		struct String retval = s1;

		retval.str[retval.length++] = c;

		return retval;
	}
	else
	{
		return EMPTY_STRING;
	}
}
