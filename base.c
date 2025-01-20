#include <string.h>

#include "base.h"


u32 mmod(i32 a, u32 n)
{
	i32 as_signed = (i32)n;
	i32 near_zero = a % as_signed;

	return (0 <= near_zero) ? near_zero : near_zero + as_signed;
}

bool zero_struct(void* struct_, u32 size)
{
	char* casted = (char*)struct_;
	for (u32 i = 0; i < size; i++)
	{
		if (!casted[i])
		{
			return false;
		}
	}

	return true;
}

struct String String_wrap(char const* cstring)
{
	return (struct String){
		.length = strlen(cstring),
		.str = cstring
	};
}

bool String_eq(struct String const left, struct String const right)
{
	if (left.length != right.length)
	{
		return false;
	}

	u32 l, r;
	char const* lstr = left.str;
	char const* rstr = right.str;

	for (l = 0, r = 0; l < left.length && lstr[l] == rstr[r]; l++, r++)
	{}

	return l == left.length;
}

bool String_ceq(struct String const left, char const* right)
{
	return String_eq(left, String_wrap(right));
}
