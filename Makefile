SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
OLEVEL = -O2
DEBUG = -g
FILES = primitives/main.c primitives/codegen.h primitives/codegen.c primitives/parsing.h primitives/parsing.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c Buffer_String.h Buffer_String.c primitives/Buffer_26_String.h primitives/Buffer_26_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c String.h String.c

.PHONY: clean tests

default: codegen

clean:
	rm tests/out/*
	rm codegen

tests: tests/out String_Test parsing_Test codegen_Test end_to_end_Test FM_Hash_Map_Int_Float_Test

tests/out:
	mkdir tests/out

String_Test: tests/out tests/String_Test.c String.h String.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

parsing_Test: tests/out tests/parsing_Test.c primitives/parsing.h primitives/parsing.c String.h String.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o ./tests/out/$@
	./tests/out/$@

codegen_Test: tests/out tests/codegen_Test.c primitives/parsing.h primitives/parsing.c primitives/codegen.h primitives/codegen.c primitives/Buffer_Parametric_Binding.h primitives/Buffer_Parametric_Binding.c Buffer_String.c Buffer_String.c primitives/Parametric_Binding.h primitives/Parametric_Binding.c String.h String.c
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
	./codegen -i templates/FM_Hash_Map_KV.h -o generated/FM_Hash_Map_Int_Float.h -K int -V float --snake-case

generated/FM_Hash_Map_Int_Float.c: codegen generated templates/FM_Hash_Map_KV.c
	./codegen -i templates/FM_Hash_Map_KV.c -o generated/FM_Hash_Map_Int_Float.c -K int -V float --snake-case

codegen: $(FILES)
	$(CC) $(CFLAGS) $(OLEVEL) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@

debug: $(FILES)
	$(CC) $(CFLAGS) $(DEBUG) $$(echo $^ | tr ' ' '\n' | grep '.c$$') -o $@
