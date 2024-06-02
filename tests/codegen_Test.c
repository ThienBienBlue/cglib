#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "../primitives/String.h"
#include "../primitives/Parametric_Binding.h"
#include "../primitives/Buffer_Parametric_Binding.h"
#include "../primitives/Buffer_String.h"

#include "../codegen.h"

struct Parametric_Binding binding(char c, char* t, char* i)
{
	return Parametric_Binding_init(c, t, i);
}

void assert_codegen(struct Codegen_Args args, char* template, char* output)
{
	struct String _template = String_wrap(template);
	struct String const generated = codegen(args, _template);

	assert((int)strlen(output) == generated.length);
	assert(strncmp(generated.str, output, generated.length) == 0);
}

int main()
{
	struct Buffer_Parametric_Binding* bindings =
			Buffer_Parametric_Binding_init(4);
	struct Buffer_String* includes = Buffer_String_init(2);
	struct Codegen_Args args;

	Buffer_Parametric_Binding_push(bindings, binding('A', "WeirdFishes",
			"struct WeirdFishes"));
	Buffer_Parametric_Binding_push(bindings, binding('B', "Idioteque",
			"struct Idioteque"));
	Buffer_Parametric_Binding_push(bindings, binding('C',
			"House_of_Cards", "struct House_of_Cards"));
	Buffer_Parametric_Binding_push(bindings, binding('T', "Int",
			"int"));

	Buffer_String_push(includes, String_wrap("String.h"));

	args.bindings = bindings;
	args.includes = includes;
	args.style = DEFAULT;

	char input1[] =
			"#ifndef TEST1\n"
			"#define TEST1\n"
			"\n"
			"void foo();\n"
			"\n"
			"#endif";
	char output1[] =
			"#ifndef TEST1\n"
			"#define TEST1\n"
			"\n"
			"#include \"String.h\"\n"
			"\n"
			"void foo();\n"
			"\n"
			"#endif";

	assert_codegen(args, input1, output1);

	char input2[] =
			"struct Type<AB,C>\n"
			"{\n"
			"\tA instanceA; B returnB();\n"
			"int const primitive;\n"
			"C* instanceC;\n"
			"}\n";
	char output2[] =
			"#include \"String.h\"\n"
			"\n"
			"struct TypeWeirdFishesIdioteque_House_of_Cards\n"
			"{\n"
			"\tstruct WeirdFishes instanceA; struct Idioteque returnB();\n"
			"int const primitive;\n"
			"struct House_of_Cards* instanceC;\n"
			"}\n";

	assert_codegen(args, input2, output2);

	args.style = SNAKE_CASE;

	char input_snake[] = "typedef struct { A f1; T f2 } Type<AT>";
	char input_snake2[] = "typedef struct { A f1; T f2 } Type_<AT>";
	char output_snake[] =
			"#include \"String.h\"\n\n"
			"typedef struct { struct WeirdFishes f1; int f2 } "
			"Type_WeirdFishes_Int";

	assert_codegen(args, input_snake, output_snake);
	assert_codegen(args, input_snake2, output_snake);

	args.style = CAMEL_CASE;

	char input_camel[] = "typedef struct { A f1; T f2 } Type<AT>";
	char input_camel2[] = "typedef struct { A f1; T f2 } Type_<A,T>";
	char output_camel[] =
			"#include \"String.h\"\n\n"
			"typedef struct { struct WeirdFishes f1; int f2 } "
			"TypeWeirdFishesInt";

	assert_codegen(args, input_camel, output_camel);
	assert_codegen(args, input_camel2, output_camel);
}
