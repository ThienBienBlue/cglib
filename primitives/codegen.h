/**
 * #include
 * - base.h
 * - bindings.h
 * - Buffer_String.h
 * - Parametric_Binding.h
 * - Buffer_Parametric_Binding.h
 */

struct Codegen_Args
{
	struct Buffer_String const* includes;
	struct Buffer_Parametric_Binding const* bindings;
	enum Code_Style style;
};

/// Returns application of :args's bindings to :template.
/// :args Bindings to apply an and includes to inject.
/// :template_string The string to generate from by applying the :bindings.
/// :returns The generated string.
struct String codegen(struct Codegen_Args const args,
		struct String const template_string);
