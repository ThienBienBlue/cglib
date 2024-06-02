/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */

struct Buffer_Parametric_Binding
{
	int capacity;
	int length;
	struct Parametric_Binding buffer[0];
};

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(int capacity);

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_filter(struct Buffer_Parametric_Binding* self, bool (*filter)(struct Parametric_Binding));

bool Buffer_Parametric_Binding_put(struct Buffer_Parametric_Binding* self, int idx, struct Parametric_Binding item);

bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item);

bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self);

bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, int left_idx, int right_idx);

struct Parametric_Binding const* Buffer_Parametric_Binding_find(
		struct Buffer_Parametric_Binding const* bindings, char c);
