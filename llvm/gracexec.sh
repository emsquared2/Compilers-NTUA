#!/bin/bash

# Function to display usage
usage() {
    echo "Usage: $0 [-O] <input file>"
    exit 1
}

# Initialize variables
optimize=""

# Parse options
while getopts ":O" opt; do
    case $opt in
        O)
            optimize="-O"
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
base_dir=$(dirname "$input_file")
base_name=$(basename "$input_file" .grc)

# Generate intermediate and final output file names
asm_file="${base_dir}/${base_name}.asm"
out_file="${base_name}.out"

# Determine the directory of the ./grace executable
grace_dir="$(dirname "$(realpath "$0")")"

# Change to the ./grace directory
pushd "$grace_dir" > /dev/null

# Run the compilation steps
./grace $optimize "$input_file"

# Check the return status of the grace command
if [ $? -ne 0 ]; then
    exit 1
fi

# Change back to the original directory
popd > /dev/null

# Check if the asm file was created
if [ ! -f "$asm_file" ]; then
    echo "Error: Assembly file $asm_file not found."
    exit 1
fi

# Create the executable
clang "$asm_file" libgrc.a -no-pie -o "$out_file"
