struct Parametric_Binding
{
	char parametric;
	struct String type_name;
	struct String type_instance;
};

struct Parametric_Binding Parametric_Binding_init(char c, char const* type_name,
		char const* type_instance);
