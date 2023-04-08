#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool variable_name_char(char c)
{
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || '_' == c;
}

int main(int argc, char* argv[])
{
	if (argc < 4)
		exit(1);

	// Slurp out template file.
	char template_buffer[2048];
	FILE* template_file = fopen(argv[1], "r");
	size_t file_size = fread(template_buffer, sizeof(char), sizeof(template_buffer), template_file);

	// Establish from argv[2] the type names used to replace <T> and _T_.
	char* type_name = argv[2];  // Used in `Array<T>'.
	char type_name_instance[100] = { 0 };  // Used in `T item;'.
	bool type_is_primitive = islower(type_name[0]);
	if (!type_is_primitive)
	{
		strcpy(type_name_instance, "struct ");
		strncpy(type_name_instance + 7, type_name, sizeof(type_name_instance) - 7);
	}
	else
	{
		strncpy(type_name_instance, type_name, sizeof(type_name_instance));
		type_name[0] = toupper(type_name[0]);
	}

	// Used to calculate the length of the header file post substituting <T> and _T_.
	int type_name_length = strnlen(type_name, 100);
	int type_name_instance_length = strnlen(type_name_instance, sizeof(type_name_instance));
	size_t header_file_length = file_size;

	// Walk over the file's contents to match <T> and _T_.
	int matches[100];
	int matches_total = 0;
	for (int idx = 0, stop = file_size - 2; idx < stop; idx++)
	{
		char c1 = template_buffer[idx];
		char c2 = template_buffer[idx + 1];
		char c3 = template_buffer[idx + 2];

		if (c1 == '<' && c2 == 'T' && c3 == '>')
		{
			header_file_length += type_name_length - 3;
			matches[matches_total++] = idx;
		}
		else if (!variable_name_char(c1) && c2 == 'T' && !variable_name_char(c3))
		{
			header_file_length += type_name_instance_length - 1;
			matches[matches_total++] = -(idx + 1);  // Yes, negative encoding.
		}
	}

	// Process of translating the template contents.
	char header_buffer[4096];
	int template_buffer_right = file_size;
	int header_buffer_right = header_file_length;
	while (0 < matches_total)
	{
		int match_idx = matches_total - 1;
		int template_idx = matches[match_idx];
		bool is_instance = false;
		if (template_idx < 0)
		{
			template_idx = -template_idx;
			is_instance = true;
		}

		int copy_amount = template_buffer_right - template_idx;
		int header_buffer_left = header_buffer_right - copy_amount;
		strncpy(header_buffer + header_buffer_left, template_buffer + template_idx, copy_amount);
		template_buffer_right = template_idx;

		copy_amount = is_instance ? type_name_instance_length : type_name_length;
		char* type_to_copy = is_instance ? type_name_instance : type_name;
		header_buffer_left += is_instance ? 1 - copy_amount : 3 - copy_amount;
		strncpy(header_buffer + header_buffer_left, type_to_copy, copy_amount);
		header_buffer_right = header_buffer_left;
		
		matches_total--;
	}
	if (0 < template_buffer_right)
		strncpy(header_buffer, template_buffer, template_buffer_right);

	// Write out the translated template file.
	FILE* header_file = fopen(argv[3], "w+");
	fwrite(header_buffer, sizeof(char), header_file_length, header_file);
	
	return 0;
}
