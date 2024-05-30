#ifndef PARSING_H_
#define PARSING_H_

#include <stdbool.h>
#include <stdio.h>

#include "./codegen.h"
#include "./primitives/Buffer_Parametric_Binding.h"
#include "./primitives/String.h"

bool is_whitespace(char c);

bool is_variable_name(char c);

struct String_Offset
{
	struct String string;
	int offset;
};

/// Reads from :str at :offset and attempts to match T with its corresponding
/// instance name.
/// :returns String of corresponding instance name if exists and number of chars
///  consumed by match. Empty String otherwise.
struct String_Offset match_instance_name(
		struct Buffer_Parametric_Binding const* bindings,
		struct String const str, int offset);

/// Reads from :str at :offset and attempts to match <ABC>, <D, E>, etc.
/// :returns a String with the name of the corresponding match and number of
///  chars consumed by match. Empty String otherwise.
struct String_Offset match_type_name(
		struct Buffer_Parametric_Binding const* bindings, enum Code_Style style,
		struct Arena* arena, struct String const str, int offset);

#endif
