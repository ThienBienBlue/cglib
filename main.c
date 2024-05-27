#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./codegen.h"
#include "./primitives/String.h"

char const* const INCLUDE = "-include";
char const* const INPUT   = "-i";
char const* const OUTPUT  = "-o";
char const* const STRUCT  = "struct ";

int const INCLUDE_LEN = 8;
int const INPUT_LEN   = 2;
int const OUTPUT_LEN  = 2;
int const STRUCT_LEN  = 7;

bool is_binding(char* arg)
{
	return strlen(arg) == 2 && arg[0] == '-' && isupper(arg[1]);
}

bool is_arg(char const* expected, char* arg)
{
	return strcmp(expected, arg) == 0;
}

struct String wrap(char const* const str)
{
	// Type casting hack. This file never modifies one of its consts.
	return String_from_cstring((char*)str);
}

int main(int argc, char* argv[])
{
	struct String arg_bindings[26] = {0};
	int num_bindings = 0;
	int struct_instance = 0;  // Malloc enough space for instance names.
	char* input = NULL;
	char* output = NULL;
	char** args_includes = NULL;
	int num_includes = 0;

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

			if (arg_bindings[idx].length == 0)
			{
				num_bindings++;
			}

			arg_bindings[idx] = wrap(argv[++i]);
			struct_instance += STRUCT_LEN;
			struct_instance += arg_bindings[idx].length;
		}
	}

	struct Arena name_arena = Arena_init(struct_instance);
	struct String struct_str = wrap(STRUCT);

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
		struct String binding = arg_bindings[i];

		if (0 < binding.length)
		{
			bool primitive = islower(binding.str[0]);
			struct String type_name;
			struct String instance_name;

			if (primitive)
			{
				instance_name = binding;
				type_name = String_init(&name_arena, binding.length);
				type_name = String_append(type_name, binding);
				type_name.str[0] += 'A' - 'a';
			}
			else
			{
				type_name = binding;
				instance_name = String_init(&name_arena,
						STRUCT_LEN + binding.length);
				instance_name = String_append(instance_name, struct_str);
				instance_name = String_append(instance_name, binding);
			}

			struct Parametric_Binding _binding;

			_binding.parametric = 'A' + i;
			_binding.type_name = type_name;
			_binding.type_instance = instance_name;

			Buffer_Parametric_Binding_push(bindings, _binding);
		}
	}

	struct Codegen_Args args;

	args.includes = includes;
	args.bindings = bindings;

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
	struct String input_str = String_init(&input_arena, input_length);

	fread(input_str.str, sizeof(char), input_length, input_file);
	input_str.length = input_length;

	struct String output_str = codegen(args, input_str);
	//if (generated == NULL)
	//{
	//	fprintf(stderr, "Unable to generate anything from the template file.\n");
	//	return 1;
	//}

	fwrite(output_str.str, sizeof(char), output_str.length, output_file);

	return 0;
}
