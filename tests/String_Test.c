#include <assert.h>
#include <stdlib.h>

#include "../base.h"
#include "../String.h"

int main()
{
	char* bytes = (char*)malloc(7);
	struct Arena arena = {
		.capacity = 7,
		.offset = 0,
		.bytes = bytes
	};
	struct String_Builder s = String_Builder_init(&arena, 1);

	struct String_Builder s1 = String_push(s, '&');

	assert(s1.str[0] == '&');
	assert(s1.length == 1);

	struct String_Builder s2 = String_push(s1, '*');

	assert(s2.length == 0);

	struct String_Builder append = String_Builder_init(&arena, -1);

	assert(append.capacity == 6);
	assert(append.length == 0);

	struct String_Builder append1 = String_append(append,
			String_Builder_build(s1));

	assert(append1.length == 1);
	assert(append1.str[0] == '&');

	struct String_Builder append2 = String_append(append1,
			String_Builder_build(append1));
	struct String_Builder append3 = String_append(append2,
			String_Builder_build(append2));

	assert(append3.length == 4);
	assert(append3.str[0] == '&');
	assert(append3.str[3] == '&');
}
