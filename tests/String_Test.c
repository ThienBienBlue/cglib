#include <assert.h>
#include <stdlib.h>

#include "../primitives/String.h"

int main()
{
	char* bytes = (char*)malloc(7);
	struct Arena arena = {
		.capacity = 7,
		.offset = 0,
		.bytes = bytes
	};
	struct String s = String_init(&arena, 1);

	struct String s1 = String_push(s, '&');

	assert(s1.str[0] == '&');
	assert(s1.length == 1);

	struct String s2 = String_push(s1, '*');
	struct String empty = String_empty();

	assert(s2.length == 0);

	struct String append = String_init(&arena, -1);

	assert(append.capacity == 6);
	assert(append.length == 0);

	struct String append1 = String_append(append, s1);

	assert(append1.length == 1);
	assert(append1.str[0] == '&');

	struct String append2 = String_append(append1, append1);
	struct String append3 = String_append(append2, append2);

	assert(append3.length == 4);
	assert(append3.str[0] == '&');
	assert(append3.str[3] == '&');
}
