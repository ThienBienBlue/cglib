#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "../String.h"
#include "./Buffer_26_String.h"

static bool write_to(struct Buffer_26_String* self, int idx, struct String item)
{
	struct String* write = self->buffer + idx;
	void* written = memcpy(write, &item, sizeof(struct String));

	return written != NULL;
}

struct Buffer_26_String Buffer_26_String_init(struct String zero)
{
	struct Buffer_26_String retval = {
		.length = 0
	};

	for (int i = 0; i < 26; i++)
	{
		write_to(&retval, i, zero);
	}

	return retval;
}

bool Buffer_26_String_put(struct Buffer_26_String* self, int idx, struct String item)
{
	if (self != NULL && 0 <= idx && idx < 26)
	{
		bool written = write_to(self, idx, item);

		if (written && self->length < idx)
		{
			self->length = idx;
		}

		return written;
	}
	else
	{
		return false;
	}
}

bool Buffer_26_String_push(struct Buffer_26_String* self, struct String item)
{
	if (self != NULL && self->length < 26)
	{
		bool written = write_to(self, self->length, item);

		if (written)
		{
			self->length++;
		}

		return written;
	}
	else
	{
		return false;
	}
}

bool Buffer_26_String_pop(struct Buffer_26_String* self)
{
	if (self != NULL && 0 < self->length)
	{
		self->length--;

		return true;
	}
	else
	{
		return false;
	}
}
