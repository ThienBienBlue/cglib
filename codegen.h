#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "./generated/Array_Char.h"

struct Codegen_Args
{
	char* input_file;
	char* output_file;
	char* parametric_bindings[26];
	int parametric_bindings_total;
	char parametric_variables[26];
};

int codegen(struct Codegen_Args args, char* includes[], int includes_length);

#endif
