#ifndef BUFFER_Parametric_Binding_H_
#define BUFFER_Parametric_Binding_H_

/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>

#include "../codegen.h"

struct Buffer_Parametric_Binding
{
	int capacity;
	int length;
	struct Parametric_Binding buffer[0];
};

struct Buffer_Parametric_Binding* Buffer_Parametric_Binding_init(int capacity);

bool Buffer_Parametric_Binding_push(struct Buffer_Parametric_Binding* self, struct Parametric_Binding item);

bool Buffer_Parametric_Binding_pop(struct Buffer_Parametric_Binding* self);

bool Buffer_Parametric_Binding_swap(struct Buffer_Parametric_Binding* self, int left_idx, int right_idx);

#endif  // BUFFER_Parametric_Binding_H_
