#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "generated/Array_Char.h"
#include "./generated/Buffer_Char.h"

#include "./parsing.h"

bool is_whitespace(char c)
{
	return ' ' == c || '\n' == c || '\t' == c;
}

bool is_variable_name(char c)
{
	return isalnum(c) || '_' == c;
}

void Buffer_Char_fread(struct Buffer_Char* self, FILE* file)
{
	self->length = fread(self->buffer, sizeof(char), self->capacity, file);
	if (self->capacity < self->length)
		self->length = self->capacity;
}

void Buffer_Char_sprintf(struct Buffer_Char* self, char* format, ...)
{
	va_list valist;
	va_start(valist, format);
	self->length = vsnprintf(self->buffer, self->capacity, format, valist);
	if (self->capacity < self->length)
		self->length = self->capacity;
	va_end(valist);
}

void Array_Char_concat_cstring(struct Array_Char *self, char *cstring, int cstring_length)
{
	struct Array_Char dummy;
	dummy.array = cstring;
	dummy.length = cstring_length;
	Array_Char_concat(self, &dummy);
}
