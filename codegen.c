#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CodeGenCliArgs.h"

#define PARAMETRICS_CAPACITY 26
#define TYPE_LENGTH 100
#define TYPE_LENGTH_NULL 101

bool variable_name_char(char c)
{
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
}

int find_char_in(char* buffer, int buffer_length, char target)
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
	struct CodeGenCliArgs cli_args = parse_cli_args(argc, argv);
	if (cli_args.input_file == NULL)
		fprintf(stderr, "Could not find an input file matching `-i ./path/to/input/file.ext'.\n");
	if (cli_args.output_file == NULL)
		fprintf(stderr, "Could not find an output file matching `-o ./path/to/output/file.ext'.\n");
	if (cli_args.parametric_bindings_total == 0)
		fprintf(stderr, "Could not pair any bindings. E.g. `-T int -R String'.\n");
	if (cli_args.input_file == NULL || cli_args.output_file == NULL || cli_args.parametric_bindings_total == 0)
		exit(1);

	// Slurp out template file. Open up header file to write to.
	FILE* template_file = fopen(cli_args.input_file, "r");
	FILE* header_file = fopen(cli_args.output_file, "w+");
	if (template_file == NULL)
		perror("Could not open the provided template file for reading.\n");
	if (header_file == NULL)
		perror("Could not open the provided generated file for writing.\n");
	if (template_file == NULL || header_file == NULL)
		exit(1);

	char template_buffer[2048];
	size_t template_length = fread(template_buffer, sizeof(char), sizeof(template_buffer), template_file);

	// Establish the type names used to replace <T> and _T_.
	int types_total = cli_args.parametric_bindings_total;
	char* type_parametrics = cli_args.parametric_variables;
	char** type_names = cli_args.parametric_bindings;
	char type_instances[PARAMETRICS_CAPACITY][TYPE_LENGTH_NULL];
	// Used to calculate the length of the header file post substituting <T> and _T_.
	int type_names_length[PARAMETRICS_CAPACITY] = { 0 };
	int type_instances_length[PARAMETRICS_CAPACITY] = { 0 };

	for (int idx = 0; idx < types_total; idx++)
	{
		char* type_name = cli_args.parametric_bindings[idx];
		char* type_instance = type_instances[idx];
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
		type_names_length[idx] = strnlen(type_name, TYPE_LENGTH);
		type_instances_length[idx] = strnlen(type_instance, TYPE_LENGTH);
	}

	// Walk over the file's contents to match <T> and _T_.
	size_t header_length = template_length;
	int matches[64];  // First character we want to write over. So idx of '<' or 'T'.
	int matches_total = 0;
	for (int idx = 0, stop = template_length - 2; idx < stop && matches_total < 64; idx++)
	{
		char c1 = template_buffer[idx];
		char c2 = template_buffer[idx + 1];
		char c3 = template_buffer[idx + 2];

		if (c1 == '<' && isupper(c2) && c3 == '>')
		{
			int type_idx = find_char_in(type_parametrics, types_total, c2);
			if (type_idx != -1)
			{
				header_length += type_names_length[type_idx] - 3;
				matches[matches_total++] = -idx;  // Yes, negative encoding
			}
		}
		else if (!variable_name_char(c1) && isupper(c2) && !variable_name_char(c3))
		{
			int type_idx = find_char_in(type_parametrics, types_total, c2);
			if (type_idx != -1)
			{
				header_length += type_instances_length[type_idx] - 1;
				matches[matches_total++] = idx + 1;  // +1 because we want idx of T in _T_ pattern.
			}
		}
	}

	// Process of translating the template contents.
	// Algorithm simply does string substitution and isn't too smart about multiple generics.
	char header_buffer[4096];
	int template_buffer_left = 0;
	int header_buffer_left = 0;
	for (int match_idx = 0; match_idx < matches_total; match_idx++)
	{
		int template_idx = matches[match_idx];
		bool is_instance = true;
		if (template_idx < 0)
		{
			template_idx = -template_idx;
			is_instance = false;
		}
		char parametric = template_buffer[template_idx + (is_instance ? 0 : 1)];
		int type_idx = find_char_in(type_parametrics, types_total, parametric);
		char* type_name = type_names[type_idx];
		char* type_instance = type_instances[type_idx];
		int type_name_length = type_names_length[type_idx];
		int type_instance_length = type_instances_length[type_idx];

		int copy_amount = template_idx - template_buffer_left;
		strncpy(header_buffer + header_buffer_left, template_buffer + template_buffer_left, copy_amount);
		header_buffer_left += copy_amount;
		template_buffer_left = template_idx + (is_instance ? 1 : 3);

		copy_amount = is_instance ? type_instance_length : type_name_length;
		char* type_to_copy = is_instance ? type_instance : type_name;
		strncpy(header_buffer + header_buffer_left, type_to_copy, copy_amount);
		header_buffer_left += copy_amount;
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
