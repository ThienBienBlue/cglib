#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Code_Gen_CLI_Args.h"

#define PARAMETRICS_CAPACITY 26
#define TYPE_LENGTH 100
#define TYPE_LENGTH_NULL 101

bool variable_name_char(char c)
{
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
}

int find_char_in(char const* buffer, int buffer_length, char target)
{
	for (int idx = 0; idx < buffer_length; idx++)
	{
		if (target == buffer[idx])
			return idx;
	}
	return -1;
}

int main(int argc, char* argv[])
{
	// Validate the arguments.
	struct Code_Gen_CLI_Args cli_args = parse_cli_args(argc, argv);
	if (NULL == cli_args.input_file)
		fprintf(stderr, "Could not find an input file matching `-i ./path/to/input/file.ext'.\n");
	if (NULL == cli_args.output_file)
		fprintf(stderr, "Could not find an output file matching `-o ./path/to/output/file.ext'.\n");
	if (0 == cli_args.parametric_bindings_total)
		fprintf(stderr, "Could not pair any bindings. E.g. `-T int -R String'.\n");
	if (NULL == cli_args.input_file || NULL == cli_args.output_file || 0 == cli_args.parametric_bindings_total)
		exit(1);

	// Slurp out the template file and open up the header file to write to.
	FILE* template_file = fopen(cli_args.input_file, "r");
	if (NULL == template_file)
		perror("Could not open the provided template file for reading");
	FILE* header_file = fopen(cli_args.output_file, "w+");
	if (NULL == header_file)
		perror("Could not open the provided generated file for writing");
	if (NULL == template_file || NULL == header_file)
		exit(1);

	fseek(template_file, 0, SEEK_END);
	long template_length = ftell(template_file);
	if (template_length <= 0)
		perror("Template file is suggested to contain <= 0 bytes. Reporting");
	char* template_buffer = (char*)malloc((int)template_length);
	if (NULL == template_buffer)
		fprintf(stderr, "Could not allocate enough memory to read in the contents of template file.\n");
	if (template_length <= 0 || NULL == template_buffer)
		exit(1);

	rewind(template_file);
	size_t template_read_amount = fread(template_buffer, sizeof(char), template_length, template_file);
	if (template_length != (long)template_read_amount)
	{
		fprintf(stderr, "Could not read the exact amount of bytes as the template file. Read in %ld/%ld bytes.\n",
				(long)template_read_amount, template_length);
		exit(1);
	}

	// Establish the type names used to replace <T> and _T_.
	int types_total = cli_args.parametric_bindings_total;
	char const* types_parametric = cli_args.parametric_variables;
	char** types_name = cli_args.parametric_bindings;
	char types_instance[PARAMETRICS_CAPACITY][TYPE_LENGTH_NULL];
	// Used to calculate the length of the header file post substituting <T> and _T_.
	int types_name_length[PARAMETRICS_CAPACITY];
	int types_instance_length[PARAMETRICS_CAPACITY];

	for (int idx = 0; idx < types_total; idx++)
	{
		char* type_name = cli_args.parametric_bindings[idx];
		char* type_instance = types_instance[idx];
		bool type_is_primitive = islower(type_name[0]);

		if (!type_is_primitive)
		{
			strcpy(type_instance, "struct ");
			strncpy(type_instance + 7, type_name, TYPE_LENGTH - 7);
		}
		else
		{
			strncpy(type_instance, type_name, TYPE_LENGTH);
			type_name[0] = toupper(type_name[0]);
		}

		types_name_length[idx] = strnlen(type_name, TYPE_LENGTH);
		types_instance_length[idx] = strnlen(type_instance, TYPE_LENGTH);
	}

	// Walk over the file's contents to match <T> and _T_.
	size_t header_length = template_length;
	int substitutions_at[64];
	int substitutions_total = 0;
	// FSM for the <A, B, C> substitution.
	enum Parametric_FSM {NONE, OPEN, PARAMETRIC} fsm_state = NONE;
	int fsm_substitution_total, fsm_substitution_at, fsm_template_length, fsm_header_length;
	fsm_template_length = fsm_header_length = 0;  // To silence warnings. Is always initialized prior to usage.

	for (int idx = 0, stop = template_length - 2; idx < stop && substitutions_total < 64; idx++)
	{
		// Two competing matchers. Both matchers can stage their changes and updates will commited at the end.
		// This is to prevent one matcher's updates messing with the other matcher's updates.
		bool match_found = false;
		int header_length_new;
		int substitutions_at_new;
		char c1 = template_buffer[idx];
		char c2 = template_buffer[idx + 1];
		char c3 = template_buffer[idx + 2];

		// _T_ matching.
		if (!variable_name_char(c1) && isupper(c2) && !variable_name_char(c3))
		{
			int type_idx = find_char_in(types_parametric, types_total, c2);
			if (type_idx != -1)
			{
				header_length_new = header_length + types_instance_length[type_idx] - 1;
				substitutions_at_new = idx + 1;  // +1 because we want idx of T in _T_ pattern.
				match_found = true;
			}
		}

		// Parametric FSM matching.
		fsm_template_length++;
		if (' ' == c1 || '\n' == c1 || '\t' == c1 || ',' == c1) {}
		else if (c1 == '<')
		{
			fsm_state = OPEN;
			fsm_substitution_total = substitutions_total;
			fsm_substitution_at = -idx;  // Purposely pick negative encoding.
			fsm_template_length = 1;
			fsm_header_length = header_length;
		}
		else if (OPEN == fsm_state || PARAMETRIC == fsm_state)
		{
			int parametric_idx;
			if (PARAMETRIC == fsm_state && '>' == c1)
			{
				fsm_state = NONE;
				match_found = true;
				substitutions_total = fsm_substitution_total;
				header_length_new = fsm_header_length - fsm_template_length;
				substitutions_at_new = fsm_substitution_at;
			}
			else if (isupper(c1) && 0 <= (parametric_idx = find_char_in(types_parametric, types_total, c1)))
			{
				fsm_state = PARAMETRIC;
				fsm_header_length += types_name_length[parametric_idx];
			}
			else
				fsm_state = NONE;
		}

		if (match_found)
		{
			header_length = header_length_new;
			substitutions_at[substitutions_total++] = substitutions_at_new;
		}
	}

	// Process of translating the template contents. Algorithm simply does string substitution.
	char* header_buffer = (char*)malloc(header_length);
	if (NULL == header_buffer)
	{
		fprintf(stderr, "Unable to allocate %d bytes to write the output file.\n", (int)header_length);
		exit(1);
	}
	int template_buffer_left = 0;
	int header_buffer_left = 0;

	for (int substitution_idx = 0; substitution_idx < substitutions_total; substitution_idx++)
	{
		int substitution_kind_and_idx = substitutions_at[substitution_idx];
		bool is_instance = 0 < substitution_kind_and_idx;
		int template_idx = (0 < substitution_kind_and_idx) ? substitution_kind_and_idx : -substitution_kind_and_idx;
		int copy_amount = template_idx - template_buffer_left;

		// Copy blurb between substitution indicies for <T, ...> and _T_.
		strncpy(header_buffer + header_buffer_left, template_buffer + template_buffer_left, copy_amount);
		header_buffer_left += copy_amount;
		template_buffer_left = template_idx;

		char parametric;
		do {  // Type instances will loop once. Otherwise we loop for length of <...>.
			parametric = template_buffer[template_buffer_left++];
			if (!isupper(parametric))
				continue;

			int type_idx = find_char_in(types_parametric, types_total, parametric);
			char* substitute_with = (is_instance) ? types_instance[type_idx] : types_name[type_idx];
			int substitute_length = (is_instance) ? types_instance_length[type_idx] : types_name_length[type_idx];

			strncpy(header_buffer + header_buffer_left, substitute_with, substitute_length);
			header_buffer_left += substitute_length;
		} while (!is_instance && parametric != '>');
	}
	if (template_buffer_left < (int)template_length)
	{
		strncpy(header_buffer + header_buffer_left, template_buffer + template_buffer_left,
				template_length - template_buffer_left);
	}

	// Write out the translated template file.
	fwrite(header_buffer, sizeof(char), header_length, header_file);

	return 0;
}
