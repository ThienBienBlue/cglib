#ifndef CODE_GEN_CLI_ARGS_H_
#define CODE_GEN_CLI_ARGS_H_

struct CodeGenCliArgs
{
	char* input_file;
	char* output_file;
	char* parametric_bindings[26];
	int parametric_bindings_total;
	char parametric_variables[26];
};

struct CodeGenCliArgs parse_cli_args(int argc, char* argv[]);

#endif  // CODE_GEN_CLI_ARGS_H_
