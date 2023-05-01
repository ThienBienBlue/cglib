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

struct Array_Char* codegen(struct Codegen_Bindings bindings, struct Array_Char template_string)
{
	// Convert the `-T String' from the CLI Args into a name `String' and instance `struct String'.
	// Convert the `-S int' from the CLI Args into a name `Int' and instance `int'.
	struct Parametric_Binding type_bindings[26];
	int bindings_total = bindings.total;

	for (int idx = 0; idx < bindings_total; idx++)
	{
		struct Buffer_Char* type_name = Buffer_Char_init(100);
		struct Buffer_Char* type_instance = Buffer_Char_init(100);

		type_bindings[idx].parametric = bindings.from[idx];
		type_bindings[idx].type_name = type_name;
		type_bindings[idx].type_instance = type_instance;

		char* type = bindings.to[idx];
		bool primitive = islower(type[0]);

		Buffer_Char_sprintf(type_name, "%s", type);
		type_name->buffer[0] = toupper(type_name->buffer[0]);
		if (primitive)
			Buffer_Char_sprintf(type_instance, "%s", type);
		else
			Buffer_Char_sprintf(type_instance, "struct %s", type);
	}

	// Init the return value. Use the template's length as an approximation for the space needed.
	struct Array_Char* generated = Array_Char_init(template_string.length);
	if (NULL == generated)
	{
		fprintf(stderr, "Unable to allocate enough memory to translate the template file into a generated file.\n");
		return NULL;
	}

	// Now copy over the template file and do parametric conversions. The following for-loop has two state variables:
	// :template_buffer_idx and :include_state. Both of these must be correctly set by end of iteration.
	char* buffer = template_string.array;
	int buffer_length = template_string.length;
	char** includes = bindings.includes;
	int includes_length = bindings.includes_length;

	int template_buffer_idx = 0;
	enum {DONE, PRIMED, LATENT} include_state = DONE;
	if (0 < includes_length || NULL == includes)
		include_state = PRIMED;

	for (int stop = buffer_length - 2; template_buffer_idx < stop; template_buffer_idx++)
	{
		char c1 = buffer[template_buffer_idx];
		char c2 = buffer[template_buffer_idx + 1];
		char c3 = buffer[template_buffer_idx + 2];

		// We also need to find a good spot to insert all the #include "...". This will be the first line with code.
		// Code is `struct ...' or `void foo()'; text of that nature. It does not mean comments, macros, and whitespace.
		if (PRIMED == include_state)
		{
			char first_non_whitespace = 0;
			for (int idx = template_buffer_idx; idx < buffer_length; idx++)
			{
				first_non_whitespace = buffer[idx];
				if ('\n' == first_non_whitespace || !is_whitespace(first_non_whitespace))
					break;
			}

			bool insert_includes = '\n' != first_non_whitespace && '#' != first_non_whitespace
					&& '*' != first_non_whitespace && '/' != first_non_whitespace;

			if (insert_includes)
			{
				for (int idx = 0; idx < includes_length; idx++)
				{
					char* include_path = includes[idx];
					int include_path_length = strlen(include_path);  // TBD: Make this safer.

					Array_Char_concat_cstring(generated, "#include \"", 10);
					Array_Char_concat_cstring(generated, include_path, include_path_length);
					Array_Char_concat_cstring(generated, "\"\n", 2);
				}
				Array_Char_push(generated, '\n');

				include_state = DONE;
			}
			else
				include_state = LATENT;
		}

		// Three competing matchers. One for type name, type instance, and a default matcher that copies over :c1.
		// First matcher that matches will commit their changes, set :idx, and continue the for-loop.
		if ('<' == c1 && isupper(c2) && (isupper(c3) || is_whitespace(c3) || ',' == c3 || '>' == c3))
		{
			enum {PARAMETRIC, CLOSED, INVALID} state = PARAMETRIC;
			int closing_idx = template_buffer_idx + 1;

			// First Pass: look forward for the '>' index, and validate the parametrics along the way.
			while (closing_idx < buffer_length && PARAMETRIC == state)
			{
				char c = buffer[closing_idx];

				if ('>' == c)
					state = CLOSED;
				else if ((isupper(c) && NULL != find_in(type_bindings, bindings_total, c)) || ',' == c
						|| is_whitespace(c))
					closing_idx++;
				else
					state = INVALID;
			}

			if (CLOSED == state)
			{
				// Second Pass: Convert the parametrics and write them into the :generated_array.
				for (int inbetween = template_buffer_idx + 1; inbetween < closing_idx; inbetween++)
				{
					char parametric = buffer[inbetween];
					struct Parametric_Binding* binding = find_in(type_bindings, bindings_total, parametric);

					if (',' == parametric)
						Array_Char_push(generated, '_');
					else if (binding != NULL)
					{
						struct Buffer_Char* type_name = binding->type_name;
						Array_Char_concat_cstring(generated, type_name->buffer, type_name->length);
					}
				}

				template_buffer_idx = closing_idx;
				continue;
			}
		}

		// Second matcher for _T_ the type instance.
		struct Parametric_Binding* _binding;
		if (!is_variable_name(c1) && !is_variable_name(c3) && isupper(c2)
				&& NULL != (_binding = find_in(type_bindings, bindings_total, c2)))
		{
			struct Buffer_Char* type_instance = _binding->type_instance;
			Array_Char_push(generated, c1);
			Array_Char_concat_cstring(generated, type_instance->buffer, type_instance->length);

			template_buffer_idx = template_buffer_idx + 1;
			continue;
		}

		// Default matcher that does nothing special.
		Array_Char_push(generated, c1);
		if (DONE != include_state && '\n' == c1)
			include_state = PRIMED;
	}
	// Deal with any remainder since the stop condition is not the full :template_buffer_length.
	while (template_buffer_idx < buffer_length)
		Array_Char_push(generated, buffer[template_buffer_idx++]);

	return generated;
}

int main(int argc, char* argv[])
{
	char* template_file_path = NULL;
	char* generated_file_path = NULL;
	struct Codegen_Bindings bindings = { 0 };

	for (int idx = 0; idx < argc; idx++)
	{
		char* arg = argv[idx];
		if (strncmp(INCLUDE_ARG, arg, INCLUDE_ARG_LENGTH) == 0)
		{
			int includes_idx = idx + 1;
			bindings.includes = argv + includes_idx;
			bindings.includes_length = 0;
			while (includes_idx < argc && argv[includes_idx][0] && '-' != argv[includes_idx][0])
			{
				bindings.includes_length++;
				includes_idx++;
			}

			idx = includes_idx - 1;
		}
		else if (strncmp(ARG_INPUT_FILE, arg, SHORT_ARG_LENGTH) == 0)
			template_file_path = argv[++idx];
		else if (strncmp(ARG_OUTPUT_FILE, arg, SHORT_ARG_LENGTH) == 0)
			generated_file_path = argv[++idx];
		else
		{
			if (!(arg[0] && arg[1] && arg[2] == 0 && arg[0] == '-' && isupper(arg[1])))
				continue;

			char parametric = arg[1];
			char* type = argv[idx + 1];
			int binding_idx = parametric - 'A';

			if (bindings.to[binding_idx] == 0)
				bindings.total++;  // Allow rebinds prefering the latest one.
			bindings.from[binding_idx] = parametric;
			bindings.to[binding_idx] = type;
		}
	}

	for (int compressed_idx = 0, search_idx = 0;
			compressed_idx < bindings.total;
			compressed_idx++, search_idx++)
	{
		while (bindings.from[search_idx] == 0)
			search_idx++;
		bindings.from[compressed_idx] = bindings.from[search_idx];
		bindings.to[compressed_idx] = bindings.to[search_idx];
	}

	// Validate the CLI Args.
	bool missing_template_file = NULL == template_file_path;
	bool missing_generated_file = NULL == generated_file_path;
	bool empty_parametric_bindings = 0 == bindings.total;

	if (missing_template_file)
		fprintf(stderr, "Missing template file. Usage `-i ./path/to/template.h'.\n");
	if (missing_generated_file)
		fprintf(stderr, "Missing generated file target. Usage `-o ./path/to/generated/h'.\n");
	if (empty_parametric_bindings)
		fprintf(stderr, "Could not find any parametric bindings. Usage `-A int -B String -C char'.\n");
	if (missing_generated_file || missing_template_file || empty_parametric_bindings)
		return 1;

	FILE* template_file = fopen(template_file_path, "r");
	if (NULL == template_file_path)
		perror("Unable to open template file for reading");
	FILE* generated_file = fopen(generated_file_path, "w");
	if (NULL == generated_file_path)
		perror("Unable to open generated file for writing");
	if (NULL == template_file_path || NULL == generated_file_path)
		return 1;

	// Read the template file into a buffer to pass into codegen.
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

	struct Array_Char from_buffer = { .array=template_buffer->buffer, .length=template_buffer->length };
	struct Array_Char* generated = codegen(bindings, from_buffer);
	if (NULL == generated)
	{
		fprintf(stderr, "Unable to generate anything from the template file.\n");
		return 1;
	}

	fwrite(generated->array, sizeof(char), generated->length, generated_file);

	return 0;
}
