#ifndef CODE_GEN_CLI_ARGS_H_
#define CODE_GEN_CLI_ARGS_H_

struct Code_Gen_CLI_Args
{
	char* input_file;
	char* output_file;
	char* parametric_bindings[26];
	char* include;
	int parametric_bindings_total;
	char parametric_variables[26];
};

/**
 * Parses a list of :argc arguments given by :argv into the
 * :returns Code_Gen_CLI_Args
 */
struct Code_Gen_CLI_Args parse_cli_args(int argc, char* argv[]);

#endif  // CODE_GEN_CLI_ARGS_H_
