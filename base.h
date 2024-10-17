#include <stdbool.h>
#include <stdint.h>

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
typedef long double f128;

struct String
{
	i32 length;
	char const* str;
};

struct String_Builder
{
	i32 capacity;
	i32 length;
	char* str;
};

struct Arena
{
	i32 capacity;
	i32 offset;
	void* bytes;
};

#ifndef __cplusplus
	#define null NULL
	#define nullptr NULL
#endif

#define UNUSED(arg) (void)(arg)

#define arraycount(a) (sizeof(a) / sizeof(*(a)))

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (b) : (a))
