#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./Buffer_Char.h"

struct Buffer_Char* Buffer_Char_init(int capacity)
{
	if (capacity < 0)
		capacity = 0;
	struct Buffer_Char* retval = (struct Buffer_Char*)malloc(sizeof(struct Buffer_Char) + capacity * sizeof(char));
	if (retval != NULL)
	{
		retval->capacity = capacity;
		retval->length = 0;
	}

	return retval;
}

bool Buffer_Char_push(struct Buffer_Char* self, char item)
{
	if (self == NULL || self->capacity <= self->length)
		return false;
	self->buffer[self->length++] = item;
	return true;
}

bool Buffer_Char_pop(struct Buffer_Char* self)
{
	if (self == NULL || self->length <= 0)
		return false;
	self->length--;
	return true;
}

bool Buffer_Char_swap(struct Buffer_Char* self, int left_idx, int right_idx)
{
	if (self == NULL)
		return false;

	int buffer_length = self->length;
	char* buffer = self->buffer;
	if (!(0 <= left_idx && left_idx < buffer_length && 0 <= right_idx && right_idx < buffer_length))
		return false;

	char temp = buffer[left_idx];
	buffer[left_idx] = buffer[right_idx];
	buffer[right_idx] = temp;

	return true;
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
