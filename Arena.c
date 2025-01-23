#include <stdlib.h>
#include <strings.h>

#include "base.h"
#include "Arena.h"


function struct Arena Arena_init(u32 capacity)
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

function struct Arena Arena_free_all(struct Arena const self)
{
	return (struct Arena){ self.capacity, 1, self.bytes };
}

function struct Arena_Alloc Arena_malloc(struct Arena* self, u32 alignment,
		i32 size)
{
	// Ensure proper pointer alignment.
	uintptr_t offset = (uintptr_t)(self->bytes + self->offset);
	uintptr_t aligned_offset = (offset + alignment - 1) & (~(alignment - 1));
	// TBD: UB with dropping the upper bits? sizeof(u32) < sizeof(uintptr_t)
	u32 aligned_offset_ = aligned_offset - (uintptr_t)self->bytes;

	if (size < 0)
	{
		u32 alloc_capacity = self->capacity - aligned_offset_;

		self->offset = self->capacity;
		bzero(self->bytes + aligned_offset_, alloc_capacity);

		return (struct Arena_Alloc){ alloc_capacity, aligned_offset_ };
	}
	else if (aligned_offset_ + size <= self->capacity)
	{
		self->offset = aligned_offset_ + size;
		bzero(self->bytes + aligned_offset_, size);

		return (struct Arena_Alloc){ size, aligned_offset_ };
	}
	else
	{
		return (struct Arena_Alloc){ 0 };
	}
}

function struct Arena_Alloc Arena_alloc(struct Arena* self, i32 size)
{
	return Arena_malloc(self, DEFAULT_ALIGNMENT, size);
}

function struct Arena_Alloc Arena_pack(struct Arena* self, i32 size)
{
	return Arena_malloc(self, 1, size);
}
