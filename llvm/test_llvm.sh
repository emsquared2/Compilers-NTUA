#!/bin/bash

# This script runs the gracexec.sh executable on all .grc files in a specified directory
# and reports the number of successful and failed executions.

# Usage: ./script.sh <program_directory>

# Note: Comment out line 435 in parser.y

# Check if the directory argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <program_directory>"
    exit 1
fi

# Counter for correct program results
successes=0

# Array to track failed programs
declare -a failures

# Paths to the directory and executable
program_dir="$1"
grace_executable="./gracexec.sh"

# Total programs counter
total_programs=0

# Check each .grc file in the programs directory
for file in "${program_dir}"*.grc; do
    ((total_programs++))  # Increment total programs counter

    # Run the grace executable with this file and capture the output
    result=$( "$grace_executable" -o "$file" )

    # Extract the last line of the result
    last_line=$(echo "$result" | tail -n 1)

    # Remove spaces from the last line and check if it matches "Success."
    last_line_clean=$(echo "$last_line" | tr -d '[:space:]')
    if [[ "$last_line_clean" == "Success." ]]; then
        ((successes++))
    else
        failures+=("$(basename "$file")")
    fi
done

echo "--------------------------"

# Print results
echo "$successes / $total_programs programs succeeded."

if [ ${#failures[@]} -ne 0 ]; then
    echo "The following programs failed:"
    for failure in "${failures[@]}"; do
        echo "$failure"
    done
fi
