#include <stdlib.h>
#include <strings.h>

#include "base.h"
#include "Arena.h"


struct Arena Arena_init(u32 capacity)
{
	u32 leading_terminator = capacity + 1;
	void* bytes = malloc(leading_terminator);

	if (bytes)
	{
		return (struct Arena){ leading_terminator, 1, bytes };
	}
	else
	{
		return (struct Arena){ 0 };
	}
}

struct Arena Arena_free_all(struct Arena const self)
{
	return (struct Arena){ self.capacity, 1, self.bytes };
}

struct Arena_Alloc Arena_malloc(struct Arena* self, u32 alignment, i32 size)
{
	// Ensure proper pointer alignment.
	uintptr_t offset = (uintptr_t)(self->bytes + self->offset);
	uintptr_t aligned_offset = (offset + alignment - 1) & (~(alignment - 1));
	u32 _aligned_offset = aligned_offset - (uintptr_t)self->bytes;

	if (size < 0)
	{
		u32 alloc_capacity = self->capacity - _aligned_offset;

		self->offset = self->capacity;
		bzero(self->bytes + _aligned_offset, alloc_capacity);

		return (struct Arena_Alloc){ alloc_capacity, _aligned_offset };
	}
	else if (_aligned_offset + size <= self->capacity)
	{
		self->offset = _aligned_offset + size;
		bzero(self->bytes + _aligned_offset, size);

		return (struct Arena_Alloc){ size, _aligned_offset };
	}
	else
	{
		return (struct Arena_Alloc){ 0 };
	}
}

struct Arena_Alloc Arena_alloc(struct Arena* self, i32 size)
{
	return Arena_malloc(self, DEFAULT_ALIGNMENT, size);
}

struct Arena_Alloc Arena_pack(struct Arena* self, i32 size)
{
	return Arena_malloc(self, 1, size);
}
