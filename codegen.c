#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./generated/Array_Char.h"
#include "./generated/Buffer_Char.h"

#include "./codegen.h"
#include "./parsing.h"

int const SHORT_ARG_LENGTH = strlen("-T");
char const ARG_INPUT_FILE[] = "-i";
char const ARG_OUTPUT_FILE[] = "-o";
int const INCLUDE_ARG_LENGTH = strlen("-include");
char const INCLUDE_ARG[] = "-include";

struct Parametric_Binding
{
	char parametric;
	struct Buffer_Char* type_name;
	struct Buffer_Char* type_instance;
};

struct Parametric_Binding* find_in(struct Parametric_Binding* array, int n, char parametric)
{
	for (int idx = 0; idx < n; idx++)
	{
		char matching_parametric = array[idx].parametric;
		if (parametric == matching_parametric)
			return array + idx;
	}
	return NULL;
}

int codegen(struct Codegen_Args args, char* includes[], int includes_length)
{
	bool missing_input_file = NULL == args.input_file;
	bool missing_output_file = NULL == args.output_file;
	bool empty_parametric_bindings = 0 == args.parametric_bindings_total;

	if (missing_input_file)
		fprintf(stderr, "Missing template file. Usage `-i ./path/to/template.h'.\n");
	if (missing_output_file)
		fprintf(stderr, "Missing generated file target. Usage `-o ./path/to/generated/h'.\n");
	if (empty_parametric_bindings)
		fprintf(stderr, "Could not find any parametric bindings. Usage `-A int -B String -C char'.\n");
	if (missing_input_file || missing_output_file || empty_parametric_bindings)
		return 1;

	FILE* template_file = fopen(args.input_file, "r");
	if (NULL == template_file)
		perror("Unable to open template file for reading");
	FILE* generated_file = fopen(args.output_file, "w");
	if (NULL == generated_file)
		perror("Unable to open generated file for writing");
	if (NULL == template_file || NULL == generated_file)
		return 1;

	// Since we have the file on hand, get the file length and pre-allocate to fit it in memory.
	fseek(template_file, 0L, SEEK_END);
	int template_length = ftell(template_file);
	int template_file_error_code = ferror(template_file);
	if (template_length < 0 || template_file_error_code)
	{
		fprintf(stderr, "Unable to gaige the length of the template file. Exiting with %d.\n",
				template_file_error_code);
		return template_file_error_code;
	}
	rewind(template_file);

	struct Buffer_Char* template_buffer = Buffer_Char_init(template_length);
	if (NULL == template_buffer)
	{
		fprintf(stderr, "Unable to allocate enough memory to read the full template file.\n");
		return 1;
	}
	Buffer_Char_fread(template_buffer, template_file);

	// Convert the `-T String' from the CLI Args into a name `String' and instance `struct String'.
	// Convert the `-S int' from the CLI Args into a name `Int' and instance `int'.
	struct Parametric_Binding type_bindings[26];
	int type_bindings_total = args.parametric_bindings_total;
	for (int idx = 0; idx < type_bindings_total; idx++)
	{
		struct Buffer_Char* type_name = Buffer_Char_init(100);
		struct Buffer_Char* type_instance = Buffer_Char_init(100);

		type_bindings[idx].parametric = args.parametric_variables[idx];
		type_bindings[idx].type_name = type_name;
		type_bindings[idx].type_instance = type_instance;

		char* arg_type_name = args.parametric_bindings[idx];
		bool primitive = islower(arg_type_name[0]);

		Buffer_Char_sprintf(type_name, "%s", arg_type_name);
		type_name->buffer[0] = toupper(type_name->buffer[0]);
		if (primitive)
			Buffer_Char_sprintf(type_instance, "%s", arg_type_name);
		else
			Buffer_Char_sprintf(type_instance, "struct %s", arg_type_name);
	}

	// Read template buffer into the generated array, converting parametric bindings along the way.
	struct Array_Char* generated_array = Array_Char_init(template_length);
	if (NULL == generated_array)
	{
		fprintf(stderr, "Unable to allocate enough memory to translate the template file into a generated file.\n");
		return 1;
	}

	// First insert in any #include "..." after initial preprocessor macros and comments.
	char* _buffer = template_buffer->buffer;
	int template_buffer_idx = 0;

	while (true)
	{
		int eol_idx = template_buffer_idx;
		enum {WHITESPACE, CODE} first_char_of_line = WHITESPACE;

		while (eol_idx < template_length)
		{
			char c = _buffer[eol_idx++];
			if ('\n' == c)
				break;
		}

		for (int idx = template_buffer_idx; idx < eol_idx; idx++)
		{
			char c = _buffer[idx];
			if ('\n' == c || '#' == c || '*' == c || '/' == c)
				break;
			else if (is_whitespace(c)) {}
			else
			{
				first_char_of_line = CODE;
				break;
			}
		}

		if (CODE == first_char_of_line)
			break;
		else
		{
			for (int idx = template_buffer_idx; idx < eol_idx; idx++)
				Array_Char_push(generated_array, _buffer[idx]);

			template_buffer_idx = eol_idx;
		}
	}

	if (0 < includes_length)
	{
		for (int idx = 0; idx < includes_length; idx++)
		{
			char* include_path = includes[idx];
			int include_path_length = strlen(include_path);  // TBD: Make this safer.

			Array_Char_concat_cstring(generated_array, "#include \"", 10);
			Array_Char_concat_cstring(generated_array, include_path, include_path_length);
			Array_Char_concat_cstring(generated_array, "\"\n", 2);
		}
		Array_Char_push(generated_array, '\n');
	}

	// Now copy over the template file and do parametric conversions.
	for (int stop = template_length - 2; template_buffer_idx < stop; template_buffer_idx++)
	{
		char c1 = _buffer[template_buffer_idx];
		char c2 = _buffer[template_buffer_idx + 1];
		char c3 = _buffer[template_buffer_idx + 2];

		// Three competing matchers. One for name, instance, and a default matcher that copies over :c1.
		// First matcher that matches will commit their changes, set :idx, and continue the for-loop.
		if ('<' == c1 && isupper(c2) && (isupper(c3) || is_whitespace(c3) || ',' == c3 || '>' == c3))
		{
			enum {PARAMETRIC, CLOSED, INVALID} state = PARAMETRIC;
			int closing_idx = template_buffer_idx + 2;

			// First Pass: look forward for the '>' index, and validate the parametrics along the way.
			while (closing_idx < template_length)
			{
				char c = _buffer[closing_idx];
				if ('>' == c)
				{
					state = CLOSED;
					break;
				}
				else if ((isupper(c) && NULL != find_in(type_bindings, type_bindings_total, c)) || ',' == c
						|| is_whitespace(c))
					closing_idx++;
				else
				{
					state = INVALID;
					break;
				}
			}

			if (CLOSED == state)
			{
				// Second Pass: Convert the parametrics and write them into the :generated_array.
				for (int inbetween = template_buffer_idx + 1; inbetween < closing_idx; inbetween++)
				{
					char parametric = _buffer[inbetween];
					struct Parametric_Binding* binding = find_in(type_bindings, type_bindings_total, parametric);

					if (',' == parametric)
						Array_Char_push(generated_array, '_');
					else if (binding != NULL)
					{
						struct Buffer_Char* type_name = binding->type_name;
						Array_Char_concat_cstring(generated_array, type_name->buffer, type_name->length);
					}
				}

				template_buffer_idx = closing_idx;
				continue;
			}
		}

		// Second matcher for _T_ the type instance.
		struct Parametric_Binding* _binding;
		if (!is_variable_name(c1) && !is_variable_name(c3) && isupper(c2)
				&& NULL != (_binding = find_in(type_bindings, type_bindings_total, c2)))
		{
			struct Buffer_Char* type_instance = _binding->type_instance;
			Array_Char_push(generated_array, c1);
			Array_Char_concat_cstring(generated_array, type_instance->buffer, type_instance->length);
			template_buffer_idx = template_buffer_idx + 1;
			continue;
		}

		// Default matcher that does nothing special.
		Array_Char_push(generated_array, c1);
	}
	// Deal with any remainder since the stop condition is not the full :template_buffer_length
	while (template_buffer_idx < template_length)
		Array_Char_push(generated_array, _buffer[template_buffer_idx++]);

	fwrite(generated_array->array, sizeof(char), generated_array->length, generated_file);

	return 0;
}

int main(int argc, char* argv[])
{
	struct Codegen_Args args = { 0 };
	char** includes = NULL;
	int includes_length = 0;

	for (int idx = 0; idx < argc; idx++)
	{
		char* arg = argv[idx];
		if (strncmp(INCLUDE_ARG, arg, INCLUDE_ARG_LENGTH) == 0)
		{
			int includes_idx = idx + 1;
			includes = argv + includes_idx;
			includes_length = 0;
			while (includes_idx < argc && argv[includes_idx][0] && '-' != argv[includes_idx][1])
			{
				includes_length++;
				includes_idx++;
			}
		}
		else if (strncmp(ARG_INPUT_FILE, arg, SHORT_ARG_LENGTH) == 0)
			args.input_file = argv[++idx];
		else if (strncmp(ARG_OUTPUT_FILE, arg, SHORT_ARG_LENGTH) == 0)
			args.output_file = argv[++idx];
		else
		{
			if (!(arg[0] && arg[1] && arg[2] == 0 && arg[0] == '-' && isupper(arg[1])))
				continue;

			char bound_parametric = arg[1];
			char* bound_type = argv[idx + 1];
			int binding_idx = bound_parametric - 'A';

			if (args.parametric_bindings[binding_idx] == 0)
				args.parametric_bindings_total++;  // Allow rebinds prefering the latest one.
			args.parametric_variables[binding_idx] = bound_parametric;
			args.parametric_bindings[binding_idx] = bound_type;
		}
	}

	for (int compressed_idx = 0, search_idx = 0;
			compressed_idx < args.parametric_bindings_total;
			compressed_idx++, search_idx++)
	{
		while (args.parametric_variables[search_idx] == 0)
			search_idx++;
		args.parametric_variables[compressed_idx] = args.parametric_variables[search_idx];
		args.parametric_bindings[compressed_idx] = args.parametric_bindings[search_idx];
	}

	return codegen(args, includes, includes_length);
}
