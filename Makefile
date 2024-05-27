SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
DEBUG = -g

.PHONY: clean tests

default: codegen

clean:
	rm tests/out/String_Test
	rm tests/out/parsing_Test
	rm tests/out/codegen_Test

tests: tests/out tests/out/String_Test tests/out/parsing_Test tests/out/codegen_Test
	./tests/out/String_Test
	./tests/out/parsing_Test
	./tests/out/codegen_Test

tests/out:
	mkdir tests/out

tests/out/String_Test: tests/out tests/String_Test.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

tests/out/parsing_Test: tests/out tests/parsing_Test.c parsing.h parsing.c primitives/String.h primitives/String.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

tests/out/codegen_Test: tests/out tests/codegen_Test.c parsing.h parsing.c codegen.h codegen.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_String.c primitives/Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

codegen: main.c codegen.h codegen.c parsing.h parsing.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_String.h primitives/Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@
