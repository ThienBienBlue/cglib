#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./bootstrap/Code_Gen_CLI_Args.h"
#include "./generated/Array_Char.h"
#include "./generated/Buffer_Char.h"

#include "./parsing.h"

struct Parametric_Binding
{
	char parametric;
	struct Buffer_Char* type_name;
	struct Buffer_Char* type_instance;
};

/// fread function for a Buffer_Char. Since fread returns the amount it *would have* returned, the buffer's length is
/// its capacity if fread could write beyond that.
void Buffer_Char_fread(struct Buffer_Char* self, FILE* file)
{
	self->length = fread(self->buffer, sizeof(char), self->capacity, file);
	if (self->capacity < self->length)
		self->length = self->capacity;
}

/// sprintf function for a Buffer_Char. Since snprintf returns the amount it *would have* returned, the buffer's length
/// is its capacity if snprintf could write beyond that.
void Buffer_Char_sprintf(struct Buffer_Char* self, char* format, ...)
{
	va_list valist;
	va_start(valist, format);
	self->length = vsnprintf(self->buffer, self->capacity, format, valist);
	if (self->capacity < self->length)
		self->length = self->capacity;
	va_end(valist);
}

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

int main(int argc, char* argv[])
{
	struct Code_Gen_CLI_Args args = parse_cli_args(argc, argv);
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
			exit(1);
	}

	FILE* template_file = fopen(args.input_file, "r");
	if (NULL == template_file)
		perror("Unable to open template file for reading");
	FILE* generated_file = fopen(args.output_file, "w");
	if (NULL == generated_file)
		perror("Unable to open generated file for writing");
	if (NULL == template_file || NULL == generated_file)
		exit(1);

	// Since we have the file on hand, get the file length and pre-allocate to fit it in memory.
	fseek(template_file, 0L, SEEK_END);
	int template_length = ftell(template_file);
	int template_file_error_code;
	if (template_length < 0 || (template_file_error_code = ferror(template_file)))
	{
		fprintf(stderr, "Unable to gaige the length of the template file. Exiting with %d.\n",
				template_file_error_code);
		exit(template_file_error_code);
	}
	rewind(template_file);

	struct Buffer_Char* template_buffer = Buffer_Char_init(template_length);
	if (NULL == template_buffer)
	{
		fprintf(stderr, "Unable to allocate enough memory to read the full template file.\n");
		exit(1);
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
		exit(1);
	}

	char* _buffer = template_buffer->buffer;
	int template_buffer_idx = 0;
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
						struct Array_Char type_name = {
							.array = binding->type_name->buffer,
							.length = binding->type_name->length,
							.capacity = binding->type_name->capacity
						};
						Array_Char_concat(generated_array, &type_name);
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
			struct Array_Char type_instance = {
				.array = _binding->type_instance->buffer,
				.length = _binding->type_instance->length,
				.capacity = _binding->type_instance->capacity
			};

			Array_Char_push(generated_array, c1);
			Array_Char_concat(generated_array, &type_instance);
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
