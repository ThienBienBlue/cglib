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
	i32 capacity;
	i32 length;
	struct Parametric_Binding buffer[0];
};

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(i32 capacity);

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_from_ptr(i32 capacity, struct Parametric_Binding* ptr);

#define Buffer_Parametric_Binding_from_carray(a) Buffer_Parametric_Binding_from_ptr(arraylen(a), (a))

function struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_filter(struct Buffer_Parametric_Binding* self, bool (*filter)(struct Parametric_Binding));

function bool Buffer_Parametric_Binding_put(struct Buffer_Parametric_Binding* self, i32 idx, struct Parametric_Binding item);

function bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item);

function bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self);

function bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, i32 left_idx, i32 right_idx);
