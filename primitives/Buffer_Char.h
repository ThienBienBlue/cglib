#ifndef BUFFER_Char_H_
#define BUFFER_Char_H_

/**
 * This file was automatically generated to be a generic Buffer.
 * Buffers are C "arrays" that also bundles the :capacity and :length.
 */
#include <stdbool.h>
#include <stdio.h>

struct Buffer_Char
{
	int capacity;
	int length;
	char buffer[0];
};

struct Buffer_Char* Buffer_Char_init(int capacity);

bool Buffer_Char_push(struct Buffer_Char* self, char item);

bool Buffer_Char_pop(struct Buffer_Char* self);

bool Buffer_Char_swap(struct Buffer_Char* self, int left_idx, int right_idx);

/// fread function for a Buffer_Char. Since fread returns the amount it *would have* returned, the buffer's length is
/// its capacity if fread could write beyond that.
void Buffer_Char_fread(struct Buffer_Char* self, FILE* file);

/// sprintf function for a Buffer_Char. Since snprintf returns the amount it *would have* returned, the buffer's length
/// is its capacity if snprintf could write beyond that.
void Buffer_Char_sprintf(struct Buffer_Char* self, char* format, ...);

#endif  // BUFFER_Char_H_
