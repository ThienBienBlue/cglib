#!/bin/bash

set -e

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
        echo "$2" > "tests/out/$1.log"
    else
        echo -e "${GREEN}$1: PASSED${NC}"
    fi
}

awk_last_include='BEGIN{last_include=0} /^#include/{last_include=NR} END{print(last_include)}'

./codegen -i templates/Buffer_T.h -o tests/out/Buffer_String.h --snake-case -T String
comp_diff=$(diff $(find . -name 'Buffer_String.h' \! -path '**/tests/out/**') tests/out/Buffer_String.h)
test_result Buffer_String.h "$comp_diff"

./codegen -i templates/Buffer_T.c -o tests/out/Buffer_String.c --snake-case -T String
buffer_string_git_path=$(find . -name 'Buffer_String.c' \! -path '**/tests/out/**')
last_include_git=$(awk "$awk_last_include" "$buffer_string_git_path")
last_include_gen=$(awk "$awk_last_include" tests/out/Buffer_String.c)
comp_diff=$(diff <(tail -n "+$last_include_git" "$buffer_string_git_path") <(tail -n "+$last_include_git" tests/out/Buffer_String.c))
test_result Buffer_String.c "$comp_diff"
