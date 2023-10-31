#!/bin/bash

# This script tests the Grace parser using programs located in the "progs" directory.
# Make sure you have the 'bc' package installed (you can install it with 'sudo apt-get install bc').

# Set the directory where the programs are located
progs_dir="progs"

# Initialize counters
total_files=0
success_count=0

# Specify the full path to the grace-parser executable
grace_executable="../parser/grace"

# Check if the grace executable exists
if [ ! -x "$grace_executable" ]; then
  echo "Error: 'grace' executable not found. Make sure it's in the correct location."
  exit 1
fi

# Iterate through program files in the specified directory
for program_file in "$progs_dir"/*; do
  if [ -f "$program_file" ]; then
    total_files=$((total_files + 1))

    # Test each program using the grace executable
    result="$("$grace_executable" < "$program_file")"

    if [[ $result == "Success." ]]; then
      success_count=$((success_count + 1))
    fi
  fi
done

# Calculate accuracy percentage and format it
if [ $total_files -gt 0 ]; then
  accuracy_percentage=$(echo "scale=2; $success_count * 100 / $total_files" | bc)
else
  accuracy_percentage=0.00
fi

# Display results
echo "Total Files: $total_files"
echo "Success Count: $success_count"
echo "Accuracy: $accuracy_percentage%"
