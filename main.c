#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./String.h"
#include "./primitives/Parametric_Binding.h"
#include "./Buffer_String.h"
#include "./primitives/Buffer_26_String.h"
#include "./primitives/Buffer_Parametric_Binding.h"

#include "./codegen.h"

char const* const INCLUDE = "-include";
char const* const INPUT   = "-i";
char const* const OUTPUT  = "-o";
char const* const STRUCT  = "struct ";
char const* const CAMEL_CASE_ARG = "--camel-case";
char const* const SNAKE_CASE_ARG = "--snake-case";

int const INCLUDE_LEN = 8;
int const INPUT_LEN   = 2;
int const OUTPUT_LEN  = 2;
int const STRUCT_LEN  = 7;

enum Binding_Type
{
	PRIMITIVE,
	COMPOUND,
	LITERAL
};

struct String_String
{
	struct String s1;
	struct String s2;
};

static bool is_binding(char* arg)
{
	return strlen(arg) == 2 && arg[0] == '-' && isupper(arg[1]);
}

static bool is_arg(char const* expected, char* arg)
{
	return strcmp(expected, arg) == 0;
}

static struct String wrap(char const* const str)
{
	return String_wrap(str);
}

struct String_String type_instance(struct Arena* arena, struct String arg)
{
	if (arg.length <= 0)
	{
		return (struct String_String) {0};
	}
	if (!isalpha(arg.str[0]))
	{
		return (struct String_String) {arg, arg};
	}

	int ptr_offset = 0;

	while (ptr_offset < arg.length
			&& arg.str[arg.length - ptr_offset - 1] == '*')
	{
		ptr_offset++;
	}

	struct String type;
	struct String instance;

	if (islower(arg.str[0]))
	{
		// Primitive case
		struct String_Builder _type = String_Builder_init(arena, arg.length);

		_type = String_append(_type, arg);
		_type.str[0] += 'A' - 'a';

		type = String_Builder_build(_type);
		instance = arg;
	}
	else if (isupper(arg.str[0]))
	{
		// Compound type case.
		struct String_Builder _instance = String_Builder_init(arena,
				STRUCT_LEN + arg.length);

		_instance = String_append(_instance, wrap(STRUCT));
		_instance = String_append(_instance, arg);

		type = arg;
		instance = String_Builder_build(_instance);
	}
	else
	{
		type = arg;
		instance = arg;
	}

	type.length -= ptr_offset;

	return (struct String_String) {type, instance};
}

int main(int argc, char* argv[])
{
	struct Buffer_26_String arg_bindings = Buffer_26_String_init(String_empty());
	int num_bindings = 0;
	int struct_instance = 0;  // Malloc enough space for instance names.
	char* input = NULL;
	char* output = NULL;
	char** args_includes = NULL;
	int num_includes = 0;
	enum Code_Style style = DEFAULT;

	for (int i = 0; i < argc; i++)
	{
		char* arg = argv[i];

		if (is_arg(INPUT, arg))
		{
			input = argv[++i];
		}
		else if (is_arg(OUTPUT, arg))
		{
			output = argv[++i];
		}
		else if (is_arg(SNAKE_CASE_ARG, arg))
		{
			style = SNAKE_CASE;
		}
		else if (is_arg(CAMEL_CASE_ARG, arg))
		{
			style = CAMEL_CASE;
		}
		else if (is_arg(INCLUDE, arg))
		{
			args_includes = argv + i + 1;
			num_includes = argc - (i + 1);

			// -include should come at the end. No args can come after.
			for (int j = i + 1; j < argc; j++)
			{
				char* include = argv[j];

				if (is_arg(INPUT, include) || is_arg(OUTPUT, include)
						|| is_arg(INCLUDE, include) || is_binding(include))
				{
					fprintf(stderr, "-include [paths...] should come at the "
							"end. Found `%s`.\n", include);
					exit(1);
				}
			}
		}
		else if (is_binding(arg))
		{
			char binds = arg[1];
			int idx = binds - 'A';

			if (arg_bindings.buffer[idx].length == 0)
			{
				num_bindings++;
			}

			Buffer_26_String_put(&arg_bindings, idx, wrap(argv[++i]));
			struct_instance += STRUCT_LEN;
			struct_instance += arg_bindings.buffer[idx].length;
		}
	}

	struct Arena name_arena = Arena_init(struct_instance);

	struct Buffer_String* includes = Buffer_String_init(num_includes);
	struct Buffer_Parametric_Binding* bindings =
			Buffer_Parametric_Binding_init(num_bindings);

	for (int i = 0; i < num_includes; i++)
	{
		char* include = args_includes[i];

		Buffer_String_push(includes, wrap(include));
	}

	for (int i = 0; i < 26; i++)
	{
		struct String binding = arg_bindings.buffer[i];

		if (0 < binding.length)
		{
			struct String_String binding_parsed = type_instance(&name_arena,
					binding);
			struct Parametric_Binding _binding = {
				.parametric = 'A' + i,
				.type_name = binding_parsed.s1,
				.type_instance = binding_parsed.s2
			};

			Buffer_Parametric_Binding_push(bindings, _binding);
		}
	}

	struct Codegen_Args args = {
		.includes = includes,
		.bindings = bindings,
		.style = style
	};

	// Validate the CLI Args.
	if (input == NULL)
	{
		fprintf(stderr, "Missing input file. Usage `-i ./path/to/input.h`.\n");
	}
	if (output == NULL)
	{
		fprintf(stderr, "Missing output file target. Usage `-o ./path/to/output.h`.\n");
	}
	if (input == NULL || output == NULL)
	{
		exit(1);
	}

	FILE* input_file = fopen(input, "r");
	FILE* output_file = fopen(output, "w");

	if (input_file == NULL)
	{
		perror("Unable to open template file for reading");
	}
	if (output_file == NULL)
	{
		perror("Unable to open generated file for writing");
	}
	if (input_file == NULL || output_file == NULL)
	{
		exit(1);
	}

	// Read the template file into a buffer to pass into codegen.
	int input_length;
	{
		fseek(input_file, 0L, SEEK_END);
		input_length = ftell(input_file);

		int template_file_error_code = ferror(input_file);

		if (input_length < 0 || template_file_error_code)
		{
			fprintf(stderr, "Unable to gaige the length of the template file. "
					"Exiting with %d.\n", template_file_error_code);
			return template_file_error_code;
		}

		rewind(input_file);
	}

	struct Arena input_arena = Arena_init(input_length);
	struct String_Builder input_str = String_Builder_init(&input_arena, input_length);

	fread(input_str.str, sizeof(char), input_length, input_file);
	input_str.length = input_length;

	struct String output_str = codegen(args, String_Builder_build(input_str));
	//if (generated == NULL)
	//{
	//	fprintf(stderr, "Unable to generate anything from the template file.\n");
	//	return 1;
	//}

	fwrite(output_str.str, sizeof(char), output_str.length, output_file);

	return 0;
}
