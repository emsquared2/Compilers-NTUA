#!/bin/bash

# Check if the first parameter (input file) is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <input file> [output file name]"
    exit 1
fi

# Assign the base name for the output files
if [ -z "$2" ]; then
    base_name="a"
else
    base_name="$2"
fi

# Generate intermediate and final output file names
imm_file="${base_name}.imm"
asm_file="${base_name}.asm"
out_file="${base_name}.out"

# Run the compilation steps
./grace -io "$1" > "$imm_file"
llc-18 -regalloc=pbqp "$imm_file" -o "$asm_file"
clang "$asm_file" libgrc.a -no-pie -o "$out_file"