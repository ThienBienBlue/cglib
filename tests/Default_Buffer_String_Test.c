#include <assert.h>

#include "../base.h"
#include "out/Default_Buffer_String.h"


int main()
{
	struct String zero = { 0 };
	struct String empty = String_mwrap("");
	struct String nonzero = String_mwrap("alps");

	struct Default_Buffer_String* zero_buffer =
			Default_Buffer_String_init_zero(4);

	assert(String_eq(zero, Default_Buffer_String_pop(zero_buffer)));
	assert(0 == Default_Buffer_String_put(zero_buffer, 100, nonzero));
	assert(1 == zero_buffer->length);

	assert(1 == Default_Buffer_String_push(zero_buffer, empty));
	assert(2 == Default_Buffer_String_push(zero_buffer, nonzero));
	assert(3 == Default_Buffer_String_push(zero_buffer, empty));
	assert(4 == Default_Buffer_String_push(zero_buffer, nonzero));
	assert(0 == Default_Buffer_String_push(zero_buffer, empty));
	assert(0 == Default_Buffer_String_push(zero_buffer, nonzero));

	for (int i = 1; i < 5; i++)
	{
		if (i & 0b1)
		{
			assert(String_eq(empty, zero_buffer->buffer[i]));
		}
		else
		{
			assert(String_eq(nonzero, zero_buffer->buffer[i]));
		}
	}

	assert(String_eq(nonzero, Default_Buffer_String_pop(zero_buffer)));
	assert(4 == zero_buffer->length);

	struct String choc = String_mwrap("choc");
	struct Default_Buffer_String* buffer = Default_Buffer_String_init(10, choc);

	assert(String_eq(choc, Default_Buffer_String_pop(buffer)));
	assert(0 == Default_Buffer_String_put(buffer, 100, nonzero));
	assert(1 == buffer->length);

	assert(3 == Default_Buffer_String_put(buffer, 3, nonzero));
	assert(String_eq(nonzero, Default_Buffer_String_pop(buffer)));
	assert(3 == buffer->length);

	Default_Buffer_String_pop(buffer);
	assert(2 == buffer->length);
	Default_Buffer_String_pop(buffer);
	assert(1 == buffer->length);
	assert(String_eq(choc, Default_Buffer_String_pop(buffer)));
	assert(1 == buffer->length);

	return 0;
}
