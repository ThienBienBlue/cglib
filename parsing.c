#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "./parsing.h"
#include "./primitives/Buffer_Parametric_Binding.h"

bool is_whitespace(char c)
{
	return ' ' == c || '\n' == c || '\t' == c;
}

bool is_variable_name(char c)
{
	return isalnum(c) || '_' == c;
}

bool is_instance_char(char c)
{
	return !is_variable_name(c) && c != '<' && c != '>';
}


struct String match_instance_name(
		struct Buffer_Parametric_Binding const* bindings,
		struct String const str, int offset)
{
	char* s = str.str;
	char instance = s[offset];
	bool valid_prev_char = (offset - 1 < 0) || is_instance_char(s[offset - 1]);
	bool valid_next_char = (str.length <= offset + 1) ||
			is_instance_char(s[offset + 1]);
	bool valid_char = isupper(instance);

	if (valid_prev_char && valid_char && valid_next_char)
	{
		struct Parametric_Binding const* binding =
				Buffer_Parametric_Binding_find(bindings, instance);

		if (binding != NULL)
		{
			return binding->type_instance;
		}
		else
		{
			return String_empty();
		}
	}
	else
	{
		return String_empty();
	}
}

struct String match_type_name(struct Buffer_Parametric_Binding const* bindings,
		struct Arena* arena, struct String const str, int offset)
{
	char* s = str.str;

	if (s[offset] != '<')
	{
		return String_empty();
	}

	bool closed = false;
	struct String retval = String_init(arena, -1);

	for (int idx = offset + 1; idx < str.length; idx++)
	{
		char c = s[idx];

		if (c == '>')
		{
			closed = true;
			break;
		}
		else if (is_whitespace(c));
		else if (isupper(c))
		{
			struct Parametric_Binding const* binding =
					Buffer_Parametric_Binding_find(bindings, c);

			if (binding != NULL)
			{
				retval = String_append(retval, binding->type_name);
			}
			else
			{
				retval = String_push(retval, c);
			}
		}
		else if (c == ',')
		{
			retval = String_push(retval, '_');
		}
		else
		{
			break;
		}
	}

	if (closed)
	{
		return retval;
	}
	else
	{
		return String_empty();
	}
}
