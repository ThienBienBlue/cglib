#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "../base.h"
#include "../Arena.h"
#include "../String.h"
#include "../primitives/Parametric_Binding.h"
#include "../primitives/Buffer_Parametric_Binding.h"

#include "../primitives/parsing.h"

struct Arena zero;

struct Parametric_Binding binding(char c, char* type_name, char* instance_name)
{
	struct String _type_name = String_wrap(type_name);
	struct String _instance_name = String_wrap(instance_name);
	struct Parametric_Binding binding = {
		.parametric = c,
		.type_name = _type_name,
		.type_instance = _instance_name
	};

	return binding;
}

void assert_type_name(struct Buffer_Parametric_Binding const* bindings,
		enum Code_Style style, char* template, char* output)
{
	struct Arena arena = zero;
	struct String _template = String_wrap(template);
	struct String parsed =
			match_type_name(bindings, style, &arena, _template, 0).binding;

	if (output)
	{
		assert(String_ceq(parsed, output));
	}
	else
	{
		assert(iszero(parsed));
	}
}

void assert_instance_name(struct Buffer_Parametric_Binding const* bindings,
		char* template, int offset, char* output)
{
	struct String _template = String_wrap(template);
	struct String parsed =
			match_instance_name(bindings, _template, offset).binding;

	if (output != NULL)
	{
		assert(String_ceq(parsed, output));
	}
	else
	{
		assert(iszero(parsed));
	}
}

int main()
{
	zero = Arena_init(100);
	struct Buffer_Parametric_Binding* bindings =
			Buffer_Parametric_Binding_init(4);

	Buffer_Parametric_Binding_push(bindings, binding('A', "WeirdFishes",
			"struct WeirdFishes"));
	Buffer_Parametric_Binding_push(bindings, binding('B', "Idioteque",
			"struct Idioteque"));
	Buffer_Parametric_Binding_push(bindings, binding('C',
			"House_of_Cards", "struct House_of_Cards"));
	Buffer_Parametric_Binding_push(bindings, binding('T', "Int",
			"int"));

	assert_type_name(bindings, DEFAULT, "<A,B>", "WeirdFishes_Idioteque");
	assert_type_name(bindings, DEFAULT, "<B,B,B>",
			"Idioteque_Idioteque_Idioteque");
	assert_type_name(bindings, DEFAULT, "<A    B>",  "WeirdFishesIdioteque");
	assert_type_name(bindings, DEFAULT, "<B\nB\n\nB>",
			"IdiotequeIdiotequeIdioteque");

	assert_type_name(bindings, SNAKE_CASE, "<A,B>", "_WeirdFishes_Idioteque");
	assert_type_name(bindings, SNAKE_CASE, "<BB,B>",
			"_Idioteque_Idioteque_Idioteque");
	assert_type_name(bindings, SNAKE_CASE, "<BB,,,,,B>",
			"_Idioteque_Idioteque_Idioteque");
	assert_type_name(bindings, SNAKE_CASE, "<A    B>",
			"_WeirdFishes_Idioteque");
	assert_type_name(bindings, SNAKE_CASE, "_<B\nB\n\nB>",
			"_Idioteque_Idioteque_Idioteque");

	assert_type_name(bindings, CAMEL_CASE, "<A,B>", "WeirdFishesIdioteque");
	assert_type_name(bindings, CAMEL_CASE, "<BB,B>",
			"IdiotequeIdiotequeIdioteque");
	assert_type_name(bindings, CAMEL_CASE, "<BB,,,,,B>",
			"IdiotequeIdiotequeIdioteque");
	assert_type_name(bindings, CAMEL_CASE, "<A    B>",
			"WeirdFishesIdioteque");
	assert_type_name(bindings, CAMEL_CASE, "_<B\nB\n\nB>",
			"IdiotequeIdiotequeIdioteque");

	assert_type_name(bindings, DEFAULT, "<a,B>", NULL);
	assert_type_name(bindings, DEFAULT, "<B,B;B>", NULL);
	assert_type_name(bindings, DEFAULT, "<Ab>", NULL);
	assert_type_name(bindings, DEFAULT, "<B'BB>", NULL);

	assert_instance_name(bindings, "T", 0, "int");
	assert_instance_name(bindings, "(T param", 1, "int");
	assert_instance_name(bindings, "\tT* ptr", 1, "int");
	assert_instance_name(bindings, "\nC", 1, "struct House_of_Cards");
}
