SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
OLEVEL = -O2
DEBUG = -g
CASE = --snake-case
FILES = primitives/main.c primitives/codegen.h primitives/codegen.c primitives/parsing.h primitives/parsing.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c buffer_string/Buffer_String.h buffer_string/Buffer_String.c primitives/Buffer_26_String.h primitives/Buffer_26_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c base.h String.h String.c

.PHONY: clean tests buffer_string array_string

default: codegen

clean:
	rm tests/out/*
	rm codegen

tests: tests/out String_Test parsing_Test codegen_Test argparse_Test end_to_end_Test FM_Hash_Map_Int_Float_Test

tests/out:
	mkdir tests/out

String_Test: tests/out tests/String_Test.c base.h String.h String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

parsing_Test: tests/out tests/parsing_Test.c primitives/parsing.h primitives/parsing.c base.h String.h String.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

codegen_Test: tests/out tests/codegen_Test.c primitives/parsing.h primitives/parsing.c primitives/codegen.h primitives/codegen.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c buffer_string/Buffer_String.c buffer_string/Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c base.h String.h String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

argparse_Test: tests/out tests/argparse_Test.c base.h String.h String.c buffer_string/Buffer_String.h buffer_string/Buffer_String.c argparse/Buffer_String_argparse.h argparse/Buffer_String_argparse.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

end_to_end_Test: codegen
	bash ./tests/$@.sh

generated:
	mkdir generated

FM_Hash_Map_Int_Float_Test: codegen tests/FM_Hash_Map_Int_Float_Test.c generated/FM_Hash_Map_Int_Float.h generated/FM_Hash_Map_Int_Float.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

buffer_string: codegen templates/Buffer_T.h templates/Buffer_T.c
	mv buffer_string/Buffer_String.h buffer_string/Buffer_String_old.h
	mv buffer_string/Buffer_String.c buffer_string/Buffer_String_old.c
	./codegen -i templates/Buffer_T.h -o buffer_string/Buffer_String.h -T String $(CASE)
	./codegen -i templates/Buffer_T.c -o buffer_string/Buffer_String.c -T String $(CASE)

array_string: codegen templates/Array_T.h templates/Array_T.c
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
