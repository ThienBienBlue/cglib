#include <ctype.h>
#include <string.h>

#include "CodeGenCliArgs.h"

struct CodeGenCliArgs parse_cli_args(int argc, char* argv[])
{
	struct CodeGenCliArgs retval = { 0 };
	for (int idx = 0; idx < argc; idx++)
	{
		char* arg = argv[idx];
		if (strncmp("-i", arg, 2) == 0)
			retval.input_file = argv[++idx];
		else if (strncmp("-o", arg, 2) == 0)
			retval.output_file = argv[++idx];
		else
		{
			if (arg[0] && arg[1] && arg[2] == 0 && arg[0] == '-' && isupper(arg[1]))
			{
				retval.parametric_variables[retval.parametric_bindings_total] = arg[1];
				retval.parametric_bindings[retval.parametric_bindings_total++] = argv[++idx];
			}
		}
	}

	return retval;
}
