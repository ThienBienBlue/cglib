#include "./String.h"
#include "./Parametric_Binding.h"

struct Parametric_Binding Parametric_Binding_init(char c, char const* type_name,
		char const* type_instance)
{
	return (struct Parametric_Binding) {
		.parametric = c,
		.type_name = String_wrap(type_name),
		.type_instance = String_wrap(type_instance)
	};
}
