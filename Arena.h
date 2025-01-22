/**
 * #include
 * - base.h
 */


struct Arena
{
	u32 capacity;
	u32 offset;
	void* bytes;
};

struct Arena_Alloc
{
	u32 capacity;
	u32 offset;
};

global u32 const DEFAULT_ALIGNMENT = sizeof(void*);

/// Initializes an arena with :capacity.
function struct Arena Arena_init(u32 capacity);

function struct Arena Arena_wrap(u32 capacity, void* bytes);

/// Frees all memory in the arena.
function struct Arena Arena_free_all(struct Arena const self);

/// Plumbing API for allocating on an arena. Allocated bytes are always zeroed.
///
/// :self can never be null.
/// :alignment power of 2 that is caller's responsibility to obtain correctly.
/// :size amount of bytes to allocate. -1 means reserve all remaining space.
/// :return handle to :self with amount of space reserved.
function struct Arena_Alloc Arena_malloc(struct Arena* self, u32 alignment, i32 size);

/// Prefered API for allocating on :arena. Uses `alignof' for :alignment.
#define Arena_align(arena, type, count) Arena_malloc((arena), alignof(type), (count) * sizeof(type));

/// Calls `Arena_malloc' with DEFAULT_ALIGNMENT.
function struct Arena_Alloc Arena_alloc(struct Arena* self, i32 size);

/// Calls `Arena_malloc' with 1 alignment to achieve packing.
function struct Arena_Alloc Arena_pack(struct Arena* self, i32 size);
