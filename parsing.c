#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#include "./String.h"
#include "./primitives/Parametric_Binding.h"
#include "./primitives/Buffer_Parametric_Binding.h"

#include "./codegen.h"
#include "./parsing.h"

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

struct String_Offset make(struct String string, int offset)
{
	return (struct String_Offset) {
		.string = string,
		.offset = offset
	};
}

struct String_Offset match_instance_name(
		struct Buffer_Parametric_Binding const* bindings,
		struct String const str, int offset)
{
	char const* s = str.str;
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
			return make(binding->type_instance, 1);
		}
		else
		{
			return make(String_empty(), 0);
		}
	}
	else
	{
		return make(String_empty(), 0);
	}
}

struct String_Offset match_type_name(
		struct Buffer_Parametric_Binding const* bindings, enum Code_Style style,
		struct Arena* arena, struct String const str, int offset)
{
	char const* s = str.str;
	int slen = str.length;
	int consumed = 0;
	enum {UNDERSCORE, BRACE} start = BRACE;

	if (s[offset] == '<')
	{
		start = BRACE;
		consumed = 1;
	}
	else if (s[offset] == '_' && offset + 1 < slen && s[offset + 1] == '<')
	{
		start = UNDERSCORE;
		consumed = 2;
	}
	else
	{
		return make(String_empty(), 0);
	}

	bool closed = false;
	bool prefix_underscore = style == DEFAULT && start == UNDERSCORE;
	struct String_Builder retval = String_Builder_init(arena, -1);

	if (prefix_underscore)
	{
		retval = String_push(retval, '_');
	}

	for (int idx = offset + consumed; idx < slen; idx++)
	{
		char c = s[idx];

		consumed++;

		if (c == '>')
		{
			closed = true;
			break;
		}
		else if (is_whitespace(c));
		else if (isupper(c))
		{
			if (style == SNAKE_CASE)
			{
				retval = String_push(retval, '_');
			}

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
			if (style == DEFAULT)
			{
				retval = String_push(retval, '_');
			}
		}
		else
		{
			break;
		}
	}

	if (closed)
	{
		return make(String_Builder_build(retval), consumed);
	}
	else
	{
		return make(String_empty(), 0);
	}
}
