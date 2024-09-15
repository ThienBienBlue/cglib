/**
 * Quick String manipulation for short lifetime strings via an Arena.
 */

struct Arena Arena_init(int capacity);

struct String String_empty();

/// Wraps a cstring into a String.
struct String String_wrap(char const* cstring);

/// If capacity < 0, String takes up remaining space of arena.
/// If capacity = 0, return empty String.
/// If 0 < capacity, return String of capacity of possible, empty otherwise.
struct String_Builder String_Builder_init(struct Arena* arena, int capacity);

/// Attempts to append all the contents of :s2 into :s1 if it will fit within
/// capacity.
/// :returns a new String sharing the same :str buffer as :s1 if :s2 fits.
///  Otherwise EMPTY_STRING.
struct String_Builder String_append(struct String_Builder s1, struct String s2);

/// Attempts to append a single character into :s.
/// :returns a new String sharing the same :str buffer as :s if :c fits.
///  Otherwise EMPTY_STRING.
struct String_Builder String_push(struct String_Builder s, char c);

struct String String_Builder_build(struct String_Builder s);
