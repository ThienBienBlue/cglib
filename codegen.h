#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "./generated/Array_Char.h"

struct Codegen_Bindings
{
	char* to[26];
	char** includes;
	int total;
	int includes_length;
	char from[26];
};

/// Reads in a template and creates a generated string by applying the :bindings.
/// :bindings The bindings of -T to Type. Since there are only A to Z, the bindings are expected to be in alphabetical
///  order and compressed (no empty bindings in between). Any includes are optional.
/// :template_string The string to generate from by applying the :bindings.
/// :returns The generated string.
struct Array_Char* codegen(struct Codegen_Bindings bindings, struct Array_Char const template_string);

#endif
