#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../base.h"
#include "../Arena.h"
#include "../String.h"
#include "../buffer_string/Buffer_String.h"
#include "Buffer_26_String.h"
#include "Parametric_Binding.h"
#include "Buffer_Parametric_Binding.h"
#include "parsing.h"
#include "codegen.h"


struct String const INCLUDE = String_mwrap("-include");
struct String const INPUT   = String_mwrap("-i");
struct String const OUTPUT  = String_mwrap("-o");
struct String const CAMEL_CASE_ARG = String_mwrap("--camel-case");
struct String const SNAKE_CASE_ARG = String_mwrap("--snake-case");

static bool is_binding(struct String const arg)
{
	return arg.length == 2 && arg.str[0] == '-' && isupper(arg.str[1]);
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
		struct String arg = String_wrap(argv[i]);

		if (String_eq(arg, INPUT))
		{
			input = argv[++i];
		}
		else if (String_eq(arg, OUTPUT))
		{
			output = argv[++i];
		}
		else if (String_eq(arg, SNAKE_CASE_ARG))
		{
			style = SNAKE_CASE;
		}
		else if (String_eq(arg, CAMEL_CASE_ARG))
		{
			style = CAMEL_CASE;
		}
		else if (String_eq(arg, INCLUDE))
		{
			args_includes = argv + i + 1;
			num_includes = argc - (i + 1);

			// `-include' should come at the end. No args can come after.
			for (int j = i + 1; j < argc; j++)
			{
				struct String include = String_wrap(argv[j]);

				if (String_eq(include, INPUT) || String_eq(include, OUTPUT)
						|| String_eq(include, INCLUDE) || is_binding(include))
				{
					fprintf(stderr, "-include [paths...] should come at the "
							"end. Found `%.*s'.\n", include.length,
							include.str);
					exit(1);
				}
			}
		}
		else if (is_binding(arg))
		{
			char binds = arg.str[1];
			int binds_i = binds - 'A';
			struct String binding = String_wrap(argv[++i]);

			if (arg_bindings.buffer[binds_i].length == 0)
			{
				num_bindings++;
			}

			Buffer_26_String_put(&arg_bindings, binds_i, binding);
			struct_instance += STRUCT.length;
			struct_instance += arg_bindings.buffer[binds_i].length;
		}
	}

	// `-include's
	struct Buffer_String* includes = Buffer_String_init(num_includes);
	for (int i = 0; i < num_includes; i++)
	{
		char* include = args_includes[i];

		Buffer_String_push(includes, String_wrap(include));
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
			struct Name_Instance type = name_instance(&binding_arena,
					binding);
			struct Parametric_Binding _binding = {
				.parametric = 'A' + i,
				.type_name = type.name,
				.type_instance = type.instance
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
		fprintf(stderr, "Missing input file. Usage `-i ./path/to/input.h'.\n");
	}
	if (output == NULL)
	{
		fprintf(stderr, "Missing output file target. Usage `-o ./path/to/output.h'.\n");
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
