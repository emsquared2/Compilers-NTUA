#!/bin/bash

# Counter for correct program results
successes=0

# Array to track failed programs
declare -a failures

# Paths to the directory and executable
program_dir="../programs/"
grace_executable="../llvm/grace"

# Total programs counter
total_programs=0

# Check each file in the programs directory
for file in "${program_dir}"*; do
    ((total_programs++))  # Increment total programs counter

    # Run the grace executable with this file and capture the output
    result=$( "$grace_executable" < "$file" )

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
