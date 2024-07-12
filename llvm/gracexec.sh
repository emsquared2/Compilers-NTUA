#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $0 [-o] <input file>"
    exit 1
}

# Initialize variables
optimize=""

# Parse options
while getopts ":o" opt; do
    case $opt in
        o)
            optimize="-o"
            ;;
        \?)
            usage
            ;;
    esac
done

# Shift past the parsed options
shift $((OPTIND - 1))

# Check if the first parameter (input file) is provided
if [ -z "$1" ]; then
    usage
fi

input_file="$1"

# Remove the extension to get the base name (including the path)
base_name="${input_file%.grc}"

# Generate intermediate and final output file names
asm_file="${base_name}.asm"
out_file="${base_name}.out"

# Run the compilation steps
./grace $optimize "$input_file"

# Check the return status of the grace command
if [ $? -ne 0 ]; then
    exit 1
fi

# Check if the asm file was created
if [ ! -f "$asm_file" ]; then
    echo "Error: Assembly file $asm_file not found."
    exit 1
fi

# Create the executable
clang "$asm_file" libgrc.a -no-pie -o "$out_file"
