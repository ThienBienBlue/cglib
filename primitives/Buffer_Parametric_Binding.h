/**
 * Buffers are C-arrays that bundle the :capacity and :length.
 * All memory is owned by the buffer is contiguous. Freeing the buffer frees
 * all memory owned by the buffer.
 *
 * #include
 * - base.h
 */


struct Buffer_Parametric_Binding
{
	u32 capacity;
	u32 length;
	struct Parametric_Binding buffer[0];
};

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(u32 capacity);

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_copy_ptr(u32 capacity, struct Parametric_Binding* ptr);

#define Buffer_Parametric_Binding_copy_carray(a) Buffer_Parametric_Binding_copy_ptr(arraylen(a), (a))

function struct Parametric_Binding Buffer_Parametric_Binding_at(struct Buffer_Parametric_Binding const* self, i32);

function bool Buffer_Parametric_Binding_put(struct Buffer_Parametric_Binding* self, u32 idx, struct Parametric_Binding item);

function bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item);

function bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self);

function bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, u32 left_idx, u32 right_idx);
