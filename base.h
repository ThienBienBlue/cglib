#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>

//-------------
// Definitions.
//-------------

// TBD: How to guarantee any of this?
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

struct String
{
	u32 length;
	char const* str;
};

//-----------
// Functions.
//-----------

#define UNUSED(arg) (void)(arg)

#define arraylen(a) (sizeof(a) / sizeof(*(a)))
#define mstrlen(str) (sizeof(str) - 1)  // -1 to account for zero terminator.

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (b) : (a))

/// HERE BE DRAGONS - this might be UB.
/// :returns true if :struct_ consists of all zero bytes.
bool zero_bytes(void* bytes, u32 size);
#define iszero(struct_) zero_bytes(&(struct_), sizeof(struct_))

#ifndef __cplusplus
	#include <stddef.h>
	#define null NULL
	#define nullptr NULL
#endif

u32 mmod(i32 a, u32 n);  // Standard mathematical Least Positive Residue.
i32 dkmod(i32 a, i32 n);  // Donald Knuth definition where n dictates sign.

/// Wraps a cstring into a String.
struct String String_wrap(char const* cstring);
#define String_mwrap(cstring) (struct String) { mstrlen((cstring)), (cstring) }

/// Returns true if :left == :right by colloquial String comparison.
bool String_eq(struct String const left, struct String const right);
bool String_ceq(struct String const left, char const* right);
