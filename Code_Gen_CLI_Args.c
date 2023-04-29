#include <ctype.h>
#include <string.h>

#include "Code_Gen_CLI_Args.h"

int const SHORT_ARG_LENGTH = strlen("-a");
char const ARG_INPUT_FILE[] = "-i";
char const ARG_OUTPUT_FILE[] = "-o";

struct Code_Gen_CLI_Args parse_cli_args(int argc, char* argv[])
{
	struct Code_Gen_CLI_Args retval = { 0 };
	for (int idx = 0; idx < argc; idx++)
	{
		char* arg = argv[idx];
		if (strncmp(ARG_INPUT_FILE, arg, SHORT_ARG_LENGTH) == 0)
			retval.input_file = argv[++idx];
		else if (strncmp(ARG_OUTPUT_FILE, arg, SHORT_ARG_LENGTH) == 0)
			retval.output_file = argv[++idx];
		else
		{
			if (!(arg[0] && arg[1] && arg[2] == 0 && arg[0] == '-' && isupper(arg[1])))
				continue;

			char bound_parametric = arg[1];
			char* bound_type = argv[idx + 1];
			int binding_idx = bound_parametric - 'A';

			if (retval.parametric_bindings[binding_idx] == 0)
				retval.parametric_bindings_total++;  // Allow rebinds prefering the latest one.
			retval.parametric_variables[binding_idx] = bound_parametric;
			retval.parametric_bindings[binding_idx] = bound_type;
		}
	}

	for (int compressed_idx = 0, search_idx = 0;
			compressed_idx < retval.parametric_bindings_total;
			compressed_idx++, search_idx++)
	{
		while (retval.parametric_variables[search_idx] == 0)
			search_idx++;
		retval.parametric_variables[compressed_idx] = retval.parametric_variables[search_idx];
		retval.parametric_bindings[compressed_idx] = retval.parametric_bindings[search_idx];
	}

	return retval;
}
