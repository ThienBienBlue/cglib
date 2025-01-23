#include "../base.h"
#include "bindings.h"
#include "Buffer_Parametric_Binding.h"


struct Parametric_Binding const* Buffer_Parametric_Binding_find(
		struct Buffer_Parametric_Binding const *binding, char parametric)
{
	for (u32 idx = 0; idx < binding->length; idx++)
	{
		char matching_parametric = binding->buffer[idx].parametric;

		if (parametric == matching_parametric)
		{
			return binding->buffer + idx;
		}
	}

	return NULL;
}
