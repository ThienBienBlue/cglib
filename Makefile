SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
OLEVEL = -O2
DEBUG = -g
CASE = --snake-case

# Modules.
BASE = base.h base.c Arena.h Arena.c String.h String.c
PARSING = primitives/bindings.h primitives/parsing.h primitives/parsing.c
CODEGEN = primitives/codegen.h primitives/codegen.c

BUFFER_T = templates/Buffer_T.h templates/Buffer_T.c
ARRAY_T = templates/Array_T.h templates/Array_T.c
DEFAULT_BUFFER_T = templates/Default_Buffer_T.h templates/Default_Buffer_T.c

BUFFER_STRING = buffer_string/Buffer_String.h buffer_string/Buffer_String.c
BUFFER_STRING_ARGPARSE = argparse/Buffer_String_argparse.h argparse/Buffer_String_argparse.c
BUFFER_PARAMETRIC_BINDING = primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_Parametric_Binding_extra.h primitives/Buffer_Parametric_Binding_extra.c

TEST_INCLUDES = tests/includes.c
PRIMITIVES = primitives/*
FILES = $(PRIMITIVES) $(BASE) $(BUFFER_STRING)


.PHONY: clean tests buffer_string array_string

default: codegen

clean:
	rm tests/out/*
	rm codegen

tests: tests/out String_Test parsing_Test codegen_Test argparse_Test end_to_end_Test FM_Hash_Map_Int_Float_Test Default_Buffer_String_Test

tests/out:
	mkdir tests/out

String_Test: tests/out tests/String_Test.c $(BASE)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

parsing_Test: tests/out tests/parsing_Test.c $(BASE) $(PARSING) $(BUFFER_PARAMETRIC_BINDING)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

codegen_Test: tests/out tests/codegen_Test.c $(BASE) $(PARSING) $(CODEGEN) $(BUFFER_PARAMETRIC_BINDING) $(BUFFER_STRING)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

argparse_Test: tests/out tests/argparse_Test.c $(BASE) $(BUFFER_STRING) $(BUFFER_STRING_ARGPARSE)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

end_to_end_Test: codegen
	bash ./tests/$@.sh

generated:
	mkdir generated

FM_Hash_Map_Int_Float_Test: codegen tests/FM_Hash_Map_Int_Float_Test.c generated/FM_Hash_Map_Int_Float.h generated/FM_Hash_Map_Int_Float.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

generated/FM_Hash_Map_Int_Float.h: codegen generated templates/FM_Hash_Map_KV.h
	./codegen -i templates/FM_Hash_Map_KV.h -o generated/FM_Hash_Map_Int_Float.h -K int -V float $(CASE)

generated/FM_Hash_Map_Int_Float.c: codegen generated templates/FM_Hash_Map_KV.c
	./codegen -i templates/FM_Hash_Map_KV.c -o generated/FM_Hash_Map_Int_Float.c -K int -V float $(CASE)

Default_Buffer_String_Test: tests/Default_Buffer_String_Test.c $(BASE) tests/out/Default_Buffer_String.h tests/out/Default_Buffer_String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

tests/out/Default_Buffer_String.h: codegen tests/out $(BASE) $(DEFAULT_BUFFER_STRING)
	./codegen -i templates/Default_Buffer_T.h -o $@ -T String $(CASE)

tests/out/Default_Buffer_String.c: codegen tests/out $(TEST_INCLUDES) $(BASE) $(DEFAULT_BUFFER_STRING)
	./codegen -i templates/Default_Buffer_T.c -o $@ -T String $(CASE)
	cat tests/includes.c $@ > $@_tmp
	mv $@_tmp $@

buffer_string: codegen $(BUFFER_T)
	./codegen -i templates/Buffer_T.h -o $@/Buffer_String.h -T String $(CASE)
	./codegen -i templates/Buffer_T.c -o $@/Buffer_String.c -T String $(CASE)

buffer_parametric_binding: codegen $(BUFFER_T) primitives/bindings.h
	./codegen -i templates/Buffer_T.h -o primitives/Buffer_Parametric_Binding.h -T Parametric_Binding $(CASE)
	./codegen -i templates/Buffer_T.c -o primitives/Buffer_Parametric_Binding.c -T Parametric_Binding $(CASE)

array_string: codegen $(ARRAY_T)
	./codegen -i templates/Array_T.h -o $@/Array_String.h -T String $(CASE)
	./codegen -i templates/Array_T.c -o $@/Array_String.c -T String $(CASE)

codegen: $(FILES)
	$(CC) $(CFLAGS) $(OLEVEL) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

debug: $(FILES)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@
