/**
 * #include
 * - base.h
 * - Array_String.h
 */

/// Splits :args into substrings following shell lite semantics.
/// - "foo bar baz" -> ["foo", "bar", "baz"]
/// - "  foo  bar  baz  " -> ["foo", "bar", "baz"]
/// - "foo 'bar baz'" -> ["foo", "bar baz"]
/// Deviations from shell start here
/// - "'foo''bar' baz'" -> ["foo", "bar", "baz"]
/// - "foo'bar' ' baz '" -> ["foo", "bar", " baz "]
/// - "'''' 'foo" -> ["", "", "foo"]
/// Deviations prevent the need for additional mallocs.
/// E.g. foo'bar' is interpreted as foobar in shell.
struct Array_String* Array_String_shell_lite_split(struct Array_String* self,
		struct String const args);
