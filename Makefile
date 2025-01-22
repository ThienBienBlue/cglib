SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
OLEVEL = -O2
DEBUG = -g
CASE = --snake-case

# Modules.
BASE = base.h base.c Arena.h Arena.c String.h String.c
PARSING = primitives/bindings.h primitives/parsing.h primitives/parsing.c
BUFFER_T = templates/Buffer_T.h templates/Buffer_T.c
ARRAY_T = templates/Array_T.h templates/Array_T.c
BUFFER_STRING = buffer_string/Buffer_String.h buffer_string/Buffer_String.c
BUFFER_STRING_ARGPARSE = argparse/Buffer_String_argparse.h argparse/Buffer_String_argparse.c
CODEGEN = primitives/codegen.h primitives/codegen.c
BUFFER_PARAMETRIC_BINDING = primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c primitives/Buffer_Parametric_Binding_extra.h primitives/Buffer_Parametric_Binding_extra.c

PRIMITIVES = primitives/*
FILES = $(PRIMITIVES) $(BASE) $(BUFFER_STRING)

.PHONY: clean tests buffer_string array_string

default: codegen

clean:
	rm tests/out/*
	rm codegen

tests: tests/out String_Test parsing_Test codegen_Test argparse_Test end_to_end_Test FM_Hash_Map_Int_Float_Test

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

buffer_string: codegen $(BUFFER_T)
	mv buffer_string/Buffer_String.h buffer_string/Buffer_String_old.h
	mv buffer_string/Buffer_String.c buffer_string/Buffer_String_old.c
	./codegen -i templates/Buffer_T.h -o buffer_string/Buffer_String.h -T String $(CASE)
	./codegen -i templates/Buffer_T.c -o buffer_string/Buffer_String.c -T String $(CASE)

buffer_parametric_binding: codegen $(BUFFER_T) primitives/bindings.h
	./codegen -i templates/Buffer_T.h -o primitives/Buffer_Parametric_Binding.h -T Parametric_Binding $(CASE)
	./codegen -i templates/Buffer_T.c -o primitives/Buffer_Parametric_Binding.c -T Parametric_Binding $(CASE)

array_string: codegen $(ARRAY_T)
	mv array_string/Array_String.h array_string/Array_String_old.h
	mv array_string/Array_String.c array_string/Array_String_old.c
	./codegen -i templates/Array_T.h -o array_string/Array_String.h -T String $(CASE)
	./codegen -i templates/Array_T.c -o array_string/Array_String.c -T String $(CASE)

generated/FM_Hash_Map_Int_Float.h: codegen generated templates/FM_Hash_Map_KV.h
	./codegen -i templates/FM_Hash_Map_KV.h -o generated/FM_Hash_Map_Int_Float.h -K int -V float $(CASE)

generated/FM_Hash_Map_Int_Float.c: codegen generated templates/FM_Hash_Map_KV.c
	./codegen -i templates/FM_Hash_Map_KV.c -o generated/FM_Hash_Map_Int_Float.c -K int -V float $(CASE)

codegen: $(FILES)
	$(CC) $(CFLAGS) $(OLEVEL) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

debug: $(FILES)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@
