#!/bin/bash

if [[ -f codegen ]]; then
    echo "Running Tests"
elif [[ -f ../codegen ]]; then
    echo "Run this test from the root directory."
    exit 1
else
    echo "Nothing to run, codegen has not been compiled."
    exit 1
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

test_result() {
    if [[ -n "$2" ]]; then
        echo -e "${RED}$1: FAILED See tests/out/$1.log${NC}"
        echo $comp_diff > "tests/out/$1.log"
    else
        echo -e "${GREEN}$1: PASSED${NC}"
    fi
}

./codegen -i templates/Buffer_T.h -o tests/out/Buffer_String.h -T String -include ./String.h
comp_diff=$(diff primitives/Buffer_String.h tests/out/Buffer_String.h)
test_result Buffer_String.h "$comp_diff"

./codegen -i templates/Buffer_T.c -o tests/out/Buffer_String.c -T String
comp_diff=$(diff primitives/Buffer_String.c tests/out/Buffer_String.c)
test_result Buffer_String.c "$comp_diff"
