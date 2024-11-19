#include "../base.h"
#include "../array_string/Array_String.h"

#include "Array_String_argparse.h"


static char const NULL_BYTE = 0;

static char const WHITESPACE1 = ' ';
static char const WHITESPACE2 = '\t';
static char const WHITESPACE3 = '\n';

static char const QUOTE1 = '\'';
static char const QUOTE2 = '"';

struct Array_String* Array_String_shell_lite_split(struct Array_String* self,
		struct String const args)
{
	if (self == null)
	{
		return null;
	}

	for (i32 i = 0; i < args.length;)
	{
		char c = args.str[i];

		if (c == NULL_BYTE)
		{
			break;
		}
		else if (c == WHITESPACE1 || c == WHITESPACE2 || c == WHITESPACE3)
		{
			i++;
			continue;
		}

		// Found a word we should record.
		char stops[5] = {0};
		i32 stops_len = 0;

		if (c == QUOTE1)
		{
			stops[0] = QUOTE1;
			stops_len = 1;
		}
		else if (c == QUOTE2)
		{
			stops[0] = QUOTE2;
			stops_len = 1;
		}
		else
		{
			stops[0] = WHITESPACE1;
			stops[1] = WHITESPACE2;
			stops[2] = WHITESPACE3;
			stops[3] = QUOTE1;
			stops[4] = QUOTE2;
			stops_len = 5;
		}

		// Advance :j to the first character that ends this word.
		i32 j;
		for (j = i + 1; j < args.length; j++)
		{
			char jc = args.str[j];

			if (jc == NULL_BYTE)
			{
				break;
			}

			i32 stops_i;
			for (stops_i = 0; stops_i < stops_len; stops_i++)
			{
				if (jc == stops[stops_i])
				{
					break;
				}
			}

			if (stops_i < stops_len)
			{
				break;
			}
		}

		// Record this word. Move :i to the end of this word.
		i32 word_start = (c == QUOTE1 || c == QUOTE2) ? i + 1 : i;
		struct String word = {
			.length = j - word_start,
			.str = args.str + word_start
		};

		Array_String_push(self, word);
		i = (c == QUOTE1 || c == QUOTE2) ? j + 1 : j;
	}

	return self;
}
