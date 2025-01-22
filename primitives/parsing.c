#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

#include "../base.h"
#include "../Arena.h"
#include "../String.h"
#include "bindings.h"
#include "Buffer_Parametric_Binding.h"
#include "Buffer_Parametric_Binding_extra.h"
#include "parsing.h"


function bool is_whitespace(char c)
{
	return ' ' == c || '\n' == c || '\t' == c;
}

internal function bool is_variable_name(char c)
{
	return isalnum(c) || '_' == c;
}

internal function bool is_instance_char(char c)
{
	return !is_variable_name(c) && c != '<' && c != '>';
}

function struct Binding_At match_instance_name(
		struct Buffer_Parametric_Binding const* bindings,
		struct String const str, unsigned int offset)
{
	char const* s = str.str;
	char instance = s[offset];
	bool valid_prev_char = (offset == 0) || is_instance_char(s[offset - 1]);
	bool valid_next_char = (str.length <= offset + 1) ||
			is_instance_char(s[offset + 1]);
	bool valid_char = isupper(instance);

	if (valid_prev_char && valid_char && valid_next_char)
	{
		struct Parametric_Binding const* binding =
				Buffer_Parametric_Binding_find(bindings, instance);

		if (binding != NULL)
		{
			return (struct Binding_At){ binding->type.instance, 1 };
		}
		else
		{
			return (struct Binding_At){ 0 };
		}
	}
	else
	{
		return (struct Binding_At){ 0 };
	}
}

function struct Binding_At match_type_name(
		struct Buffer_Parametric_Binding const* bindings, enum Code_Style style,
		struct Arena* arena, struct String const str, int offset)
{
	char const* s = str.str;
	int slen = str.length;
	int consumed = 0;
	enum { UNDERSCORE, BRACE } start = BRACE;

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
		return (struct Binding_At){ 0 };
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
				retval = String_append(retval, binding->type.name);
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
		return (struct Binding_At){
			String_Builder_build(retval),
			consumed
		};
	}
	else
	{
		return (struct Binding_At){ 0 };
	}
}

function struct Name_Instance name_instance(struct Arena* arena,
		struct String arg)
{
	if (arg.length <= 0)
	{
		return (struct Name_Instance){ 0 };
	}
	if (!isalpha(arg.str[0]))
	{
		return (struct Name_Instance){ arg, arg };
	}

	u32 ptr_offset = 0;

	while (ptr_offset < arg.length
			&& arg.str[arg.length - ptr_offset - 1] == '*')
	{
		ptr_offset++;
	}

	struct String name;  // `Buffer<T>' -> `Buffer_String'
	struct String instance;  // `T inst' -> `struct String inst'

	if (islower(arg.str[0]))
	{
		// Primitive case.
		struct String_Builder _name = String_Builder_init(arena, arg.length);

		_name = String_append(_name, arg);
		_name.str[0] += 'A' - 'a';

		name = String_Builder_build(_name);
		instance = arg;
	}
	else if (isupper(arg.str[0]))
	{
		// struct type case.
		struct String_Builder _instance = String_Builder_init(arena,
				STRUCT.length + arg.length);

		_instance = String_append(_instance, STRUCT);
		_instance = String_append(_instance, arg);

		name = arg;
		instance = String_Builder_build(_instance);
	}
	else
	{
		name = arg;
		instance = arg;
	}

	name.length -= ptr_offset;

	return (struct Name_Instance) {name, instance};
}
