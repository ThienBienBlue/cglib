SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: clean

default: codegen

clean:
	rm tests/String_Test
	rm tests/parsing_Test

tests: tests/String_Test tests/parsing_Test
	./tests/String_Test
	./tests/parsing_Test

tests/String_Test: tests/String_Test.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

tests/parsing_Test: tests/parsing_Test.c parsing.h parsing.c primitives/String.h primitives/String.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

codegen: codegen.h codegen.c parsing.h parsing.c ./primitives/Buffer_Char.h ./primitives/Buffer_Char.c ./primitives/Array_Char.h ./primitives/Array_Char.c ./primitives/Buffer_Parametric_Binding.h ./primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) codegen.c parsing.c ./primitives/Buffer_Char.c ./primitives/Array_Char.c ./primitives/Buffer_Parametric_Binding.c -O2 -o codegen
