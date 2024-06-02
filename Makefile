SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
OLEVEL = -O3
DEBUG = -g

.PHONY: clean tests

default: codegen

clean:
	rm tests/out/*
	rm codegen

tests: tests/out String_Test parsing_Test codegen_Test end_to_end_Test

tests/out:
	mkdir tests/out

String_Test: tests/out tests/String_Test.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

parsing_Test: tests/out tests/parsing_Test.c parsing.h parsing.c primitives/String.h primitives/String.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

codegen_Test: tests/out tests/codegen_Test.c parsing.h parsing.c codegen.h codegen.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_String.c primitives/Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

end_to_end_Test: codegen
	bash ./tests/$@.sh

codegen: main.c codegen.h codegen.c parsing.h parsing.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_String.h primitives/Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(OLEVEL) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

debug: main.c codegen.h codegen.c parsing.h parsing.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_String.h primitives/Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c primitives/String.h primitives/String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@
