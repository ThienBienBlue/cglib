#include <assert.h>

#include "../base.h"
#include "../buffer_string/Buffer_String.h"
#include "../argparse/Buffer_String_argparse.h"
#include "../String.h"

int main()
{
	// Basic functionality test.
	{
		struct String test = String_wrap("foo bar baz");
		struct Buffer_String* val = Buffer_String_shell_lite_split(test);

		assert(val->length == 3
				&& "Expect \"foo bar baz\" to be parsed into 3 Strings!");
		assert(val->capacity == 3
				&& "Expect \"foo bar baz\" to be parsed into 3 Strings!");
		assert(String_ceq(val->buffer[0], "foo")
				&& "Expect \"foo bar baz\"'s first arg to be \"foo\"!");
		assert(String_ceq(val->buffer[1], "bar")
				&& "Expect \"foo bar baz\"'s second arg to be \"bar\"!");
		assert(String_ceq(val->buffer[2], "baz")
				&& "Expect \"foo bar baz\"'s third arg to be \"baz\"!");
	}

	// Test combinations of whitespaces.
	{
		struct String test = String_wrap("fool\n\nbarrel  \tbazzite");
		struct Buffer_String* val = Buffer_String_shell_lite_split(test);

		assert(val->length == 3
				&& "Expect \"fool\n\nbarrel  \tbazzite\" to be parsed into 3 Strings!");
		assert(val->capacity == 3
				&& "Expect \"fool\n\nbarrel  \tbazzite\" to be parsed into 3 Strings!");
		assert(String_ceq(val->buffer[0], "fool")
				&& "Expect \"fool\n\nbarrel  \tbazzite\"'s first arg to be \"fool\"!");
		assert(String_ceq(val->buffer[1], "barrel")
				&& "Expect \"fool\n\nbarrel  \tbazzite\"'s second arg to be \"barrel\"!");
		assert(String_ceq(val->buffer[2], "bazzite")
				&& "Expect \"fool\n\nbarrel  \tbazzite\"'s third arg to be \"bazzite\"!");
	}

	// Test the usage of quotes.
	{
		struct String test = String_wrap("'foo bar' baz");
		struct Buffer_String* val = Buffer_String_shell_lite_split(test);

		assert(val->length == 2
				&& "Expect \"'foo bar' baz to be parsed into 2 Strings!");
		assert(val->capacity == 2
				&& "Expect \"'foo bar' baz to be parsed into 2 Strings!");
		assert(String_ceq(val->buffer[0], "foo bar")
				&& "Expect \"'foo bar' baz\"'s first arg to be \"foo bar\"");
		assert(String_ceq(val->buffer[1], "baz")
				&& "Expect \"'foo bar' baz\"'s second arg to be \"baz\"");
	}

	// Deviations from shell parsing starts here.
	{
		struct String test = String_wrap("'''' '' 'foo");
		struct Buffer_String* val = Buffer_String_shell_lite_split(test);

		assert(val->length == 4
				&& "Expect \"'''' '' 'foo\" to be parsed into 4 Strings!");
		assert(val->capacity == 4
				&& "Expect \"'''' '' 'foo\" to be parsed into 4 Strings!");
		assert(String_ceq(val->buffer[0], "")
				&& "Expect \"'''' '' 'foo\"'s first arg to be \"\"");
		assert(String_ceq(val->buffer[1], "")
				&& "Expect \"'''' '' 'foo\"'s second arg to be \"\"");
		assert(String_ceq(val->buffer[2], "")
				&& "Expect \"'''' '' 'foo\"'s third arg to be \"\"");
		assert(String_ceq(val->buffer[3], "foo")
				&& "Expect \"'''' '' 'foo\"'s fourth arg to be \"foo\"");
	}

	{
		struct String test = String_wrap("'foo''bar' 'baz'");
		struct Buffer_String* val = Buffer_String_shell_lite_split(test);

		assert(val->length == 3
				&& "Expect \"'foo''bar' 'baz'\" to be parsed into 3 Strings!");
		assert(val->capacity == 3
				&& "Expect \"'foo''bar' 'baz'\" to be parsed into 3 Strings!");
		assert(String_ceq(val->buffer[0], "foo")
				&& "Expect \"'foo''bar' 'baz'\"'s first arg to be \"foo\"");
		assert(String_ceq(val->buffer[1], "bar")
				&& "Expect \"'foo''bar' 'baz'\"'s second arg to be \"bar\"");
		assert(String_ceq(val->buffer[2], "baz")
				&& "Expect \"'foo''bar' 'baz'\"'s third arg to be \"baz\"");
	}

	{
		struct String test = String_wrap("foo'bar' \n'\t\tba\nz\n'");
		struct Buffer_String* val = Buffer_String_shell_lite_split(test);

		assert(val->length == 3
				&& "Expect \"foo'bar' \n'\t\tba\nz\n'\" to be parsed into 3 Strings!");
		assert(val->capacity == 3
				&& "Expect \"foo'bar' \n'\t\tba\nz\n'\" to be parsed into 3 Strings!");
		assert(String_ceq(val->buffer[0], "foo")
				&& "Expect \"foo'bar' \n'\t\tba\nz\n'\"'s first arg to be \"foo\"");
		assert(String_ceq(val->buffer[1], "bar")
				&& "Expect \"foo'bar' \n'\t\tba\nz\n'\"'s second arg to be \"bar\"");
		assert(String_ceq(val->buffer[2], "\t\tba\nz\n")
				&& "Expect \"foo'bar' \n'\t\tba\nz\n'\"'s third arg to be \"\t\tba\nz\n\"");
	}

	return 0;
}
