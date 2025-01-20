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

struct Arena_Alloc Arena_alloc(struct Arena* self, i32 size)
{
	// Ensure proper pointer alignment.
	uintptr_t offset = (uintptr_t)(self->bytes + self->offset);
	u32 alignment = (0 < (offset & (DEFAULT_ALIGNMENT - 1)))
			? (offset & (DEFAULT_ALIGNMENT - 1))
			: 0;
	uintptr_t aligned_offset = offset + alignment;
	u32 _aligned_offset = aligned_offset - (uintptr_t)self->bytes;

	if (size < 0)
	{
		u32 alloc_capacity = self->capacity - _aligned_offset;
		self->offset = self->capacity;

		return (struct Arena_Alloc){ alloc_capacity, _aligned_offset };
	}
	else if (_aligned_offset + size <= self->capacity)
	{
		self->offset = _aligned_offset + size;

		return (struct Arena_Alloc){ size, _aligned_offset };
	}
	else
	{
		return (struct Arena_Alloc){ 0 };
	}
}

struct Arena_Alloc Arena_pack(struct Arena* self, i32 size)
{
	if (size < 0)
	{
		u32 capacity = self->capacity - self->offset;
		u32 offset = self->offset;

		self->offset = self->capacity;
		bzero(self->bytes + offset, capacity);

		return (struct Arena_Alloc){ capacity, offset };
	}
	if (self->offset + size <= self->capacity)
	{
		u32 offset = self->offset;

		self->offset += size;
		bzero(self->bytes + offset, size);

		return (struct Arena_Alloc){ size, offset };
	}
	else
	{
		return (struct Arena_Alloc){ 0 };
	}
}
