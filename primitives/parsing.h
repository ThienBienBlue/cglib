/**
 * #include
 * - base.h
 * - bindings.h
 * - Arena.h
 * - Buffer_Parametric_Binding.h
 */


static struct String const STRUCT = String_mwrap("struct ");

/// :returns true if :c is one of ' ', '\t', or '\n'.
bool is_whitespace(char c);

/// Reads from :str at :offset and attempts to match T with its corresponding
/// instance name.
/// :returns String of corresponding instance name if exists and number of chars
///  consumed by match. Empty String otherwise.
struct Binding_At match_instance_name(
		struct Buffer_Parametric_Binding const* bindings,
		struct String const str, u32 offset);

/// Reads from :str at :offset and attempts to match <ABC>, <D, E>, etc.
/// :returns a String with the name of the corresponding match and number of
///  chars consumed by match. Empty String otherwise.
struct Binding_At match_type_name(
		struct Buffer_Parametric_Binding const* bindings, enum Code_Style style,
		struct Arena* arena, struct String const str, int offset);

/// Parses :arg and pushes :arg's name and instance onto :arena.
///
/// :arena buffer to store name and instance. Should be large enough.
/// :arg corresponding to the type.
/// :returns :arg's name and instance. Empty string if :arena is undercapacity.
struct Name_Instance name_instance(struct Arena* arena, struct String arg);
