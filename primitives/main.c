#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../base.h"
#include "../Arena.h"
#include "../String.h"
#include "Parametric_Binding.h"
#include "../buffer_string/Buffer_String.h"
#include "Buffer_26_String.h"
#include "Buffer_Parametric_Binding.h"

#include "codegen.h"


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

struct Name_Instance
{
	struct String name;
	struct String instance;
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

struct Name_Instance type_instance(struct Arena* arena, struct String arg)
{
	if (arg.length <= 0)
	{
		return (struct Name_Instance){0};
	}
	if (!isalpha(arg.str[0]))
	{
		return (struct Name_Instance){arg, arg};
	}

	u32 ptr_offset = 0;

	while (ptr_offset < arg.length
			&& arg.str[arg.length - ptr_offset - 1] == '*')
	{
		ptr_offset++;
	}

	struct String name;  // `Buffer<T>' -> `Buffer_String'
	struct String instance;  // `T inst' -> `struct String inst'

	if (islower(arg.str[0]))
	{
		// Primitive case.
		struct String_Builder _name = String_Builder_init(arena, arg.length);

		_name = String_append(_name, arg);
		_name.str[0] += 'A' - 'a';

		name = String_Builder_build(_name);
		instance = arg;
	}
	else if (isupper(arg.str[0]))
	{
		// struct type case.
		struct String_Builder _instance = String_Builder_init(arena,
				STRUCT_LEN + arg.length);

		_instance = String_append(_instance, wrap(STRUCT));
		_instance = String_append(_instance, arg);

		name = arg;
		instance = String_Builder_build(_instance);
	}
	else
	{
		name = arg;
		instance = arg;
	}

	name.length -= ptr_offset;

	return (struct Name_Instance) {name, instance};
}

int main(int argc, char* argv[])
{
	struct Buffer_26_String arg_bindings = Buffer_26_String_init((struct String){ 0 });
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

			// `-include' should come at the end. No args can come after.
			for (int j = i + 1; j < argc; j++)
			{
				char* include = argv[j];

				if (is_arg(INPUT, include) || is_arg(OUTPUT, include)
						|| is_arg(INCLUDE, include) || is_binding(include))
				{
					fprintf(stderr, "-include [paths...] should come at the "
							"end. Found `%s'.\n", include);
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

	// `-include's
	struct Buffer_String* includes = Buffer_String_init(num_includes);
	for (int i = 0; i < num_includes; i++)
	{
		char* include = args_includes[i];

		Buffer_String_push(includes, wrap(include));
	}

	struct Arena binding_arena = Arena_init(struct_instance);
	struct Buffer_Parametric_Binding* bindings =
			Buffer_Parametric_Binding_init(num_bindings);

	// Resolve `-T String' -> `struct String' and `NAME_String'
	// Resolve `-T int' -> `int' and `NAME_Int'
	for (int i = 0; i < 26; i++)
	{
		struct String binding = arg_bindings.buffer[i];

		if (0 < binding.length)
		{
			struct Name_Instance binding_parsed = type_instance(&binding_arena,
					binding);
			struct Parametric_Binding _binding = {
				.parametric = 'A' + i,
				.type_name = binding_parsed.name,
				.type_instance = binding_parsed.instance
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
	struct String_Builder input_str = String_Builder_init(&input_arena,
			input_length);

	fread(input_str.str, sizeof(char), input_length, input_file);
	input_str.length = input_length;

	struct String output_str = codegen(args, String_Builder_build(input_str));

	fwrite(output_str.str, sizeof(char), output_str.length, output_file);

	return 0;
}
