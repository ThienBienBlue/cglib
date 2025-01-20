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

static u32 const DEFAULT_ALIGNMENT = 2 * sizeof(void*);

/// Initializes an arena with :capacity.
struct Arena Arena_init(u32 capacity);

struct Arena Arena_wrap(u32 capacity, void* bytes);

/// Frees all memory in the arena.
struct Arena Arena_free_all(struct Arena const self);

/// Allocates :size from :bytes with alignment as needed.
/// If :size < 0 then allocate the remaining space in the arena.
/// See also `Arena_pack'.
///
/// :returns <0 representing the offset into :bytes or 0 if no allocation.
struct Arena_Alloc Arena_alloc(struct Arena* self, i32 size);

/// Allocates :size from :bytes with no alignment.
/// If :size < 0 then allocate the remaining space in the arena.
/// See also `Arena_alloc'.
///
/// :returns <0 representing the offset into :bytes or 0 if no allocation.
struct Arena_Alloc Arena_pack(struct Arena* self, i32 size);
