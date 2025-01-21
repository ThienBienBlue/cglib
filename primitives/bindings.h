/**
 * #include
 * - base.h
 */

enum Code_Style
{
	DEFAULT,
	SNAKE_CASE,
	CAMEL_CASE
};

struct Binding_At
{
	struct String binding;
	u32 occupies;
};

/// Let T be bound to `String'.
/// :name = `String' used in `Buffer<T> -> Buffer_String'.
/// :instance = `struct String inst' -> `T inst -> struct String inst'.
///
/// For primitives `i32', :name = `I32' and :instance = `i32 inst'.
struct Name_Instance
{
	struct String name;
	struct String instance;
};

struct Parametric_Binding
{
	char parametric;
	struct Name_Instance type;
};
