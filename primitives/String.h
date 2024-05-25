#ifndef STRING_H_
#define STRING_H_

/**
 * Quick String manipulation for short lifetime strings via an Arena.
 */

struct Arena
{
	int capacity;
	int offset;
	char* bytes;
};

struct String
{
	int length;
	int capacity;
	char* str;
};

struct Arena Arena_init(int capacity);

struct String String_empty();

/// If capacity < 0, String takes up remaining space of arena.
/// If capacity = 0, return empty String.
/// If 0 < capacity, return String of capacity of possible, empty otherwise.
struct String String_init(struct Arena* arena, int capacity);

struct String String_from_cstring(char* cstring);

/// Attempts to append all the contents of :s2 into :s1 if it will fit within
/// capacity.
/// :returns a new String sharing the same :str buffer as :s1 if :s2 fits.
///  Otherwise EMPTY_STRING.
struct String String_append(struct String s1, struct String s2);

/// Attempts to append a single character into :s.
/// :returns a new String sharing the same :str buffer as :s if :c fits.
///  Otherwise EMPTY_STRING.
struct String String_push(struct String s, char c);

#endif
