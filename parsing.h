#ifndef PARSING_H_
#define PARSING_H_

#include <stdbool.h>
#include <stdio.h>

#include "./generated/Buffer_Char.h"

bool is_whitespace(char c);

bool is_variable_name(char c);

/// fread function for a Buffer_Char. Since fread returns the amount it *would have* returned, the buffer's length is
/// its capacity if fread could write beyond that.
void Buffer_Char_fread(struct Buffer_Char* self, FILE* file);

/// sprintf function for a Buffer_Char. Since snprintf returns the amount it *would have* returned, the buffer's length
/// is its capacity if snprintf could write beyond that.
void Buffer_Char_sprintf(struct Buffer_Char* self, char* format, ...);

#endif
