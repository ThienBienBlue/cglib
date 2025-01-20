bool is_whitespace(char c);

bool is_variable_name(char c);

struct Binding_At
{
	struct String binding;
	u32 occupies;
};

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
