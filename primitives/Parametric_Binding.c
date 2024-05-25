#include "./Parametric_Binding.h"
#include "./String.h"

struct Parametric_Binding Parametric_Binding_init(char c, char* type_name,
		char* type_instance)
{
	struct Parametric_Binding retval;

	retval.parametric = c;
	retval.type_name = String_from_cstring(type_name);
	retval.type_instance = String_from_cstring(type_instance);

	return retval;
}
