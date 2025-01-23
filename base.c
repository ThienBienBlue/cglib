#include <string.h>

#include "base.h"


function u32 mmod(i32 a, u32 n)
{
	i32 as_signed = (i32)n;
	i32 near_zero = a % as_signed;

	return (0 <= near_zero) ? near_zero : near_zero + as_signed;
}

function bool zero_bytes(void* bytes, u32 size)
{
	char* bytes_ = (char*)bytes;
	for (u32 i = 0; i < size; i++)
	{
		if (bytes_[i])
		{
			return false;
		}
	}

	return true;
}

function struct String String_wrap(char const* cstring)
{
	return (struct String){
		.length = strlen(cstring),
		.str = cstring
	};
}

function bool String_eq(struct String const left, struct String const right)
{
	if (left.length != right.length)
	{
		return false;
	}

	u32 i;
	char const* lstr = left.str;
	char const* rstr = right.str;

	for (i = 0; i < left.length && lstr[i] == rstr[i]; i++)
	{}

	return i == left.length;
}

function bool String_ceq(struct String const left, char const* right)
{
	return String_eq(left, String_wrap(right));
}
