#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "./primitives/Buffer_Parametric_Binding.h"
#include "./primitives/Buffer_String.h"
#include "./primitives/String.h"
#include "./primitives/Parametric_Binding.h"

struct Codegen_Args
{
	struct Buffer_String const* includes;
	struct Buffer_Parametric_Binding const* bindings;
};

/// Returns application of :args's bindings to :template.
/// :args Bindings to apply an and includes to inject.
/// :template_string The string to generate from by applying the :bindings.
/// :returns The generated string.
struct String const codegen(struct Codegen_Args const args,
		struct String const template_string);

#endif
