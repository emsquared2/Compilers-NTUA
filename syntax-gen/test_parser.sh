#!/bin/bash

# This script generates a specified number (N) of Grace programs using the "gen.sh" script.
# It then tests the Grace parser with the generated programs and displays the results using the "test.sh" script.

# Check if the number of programs to generate is provided as an argument
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <number_of_programs>"
  exit 1
fi

# Number of programs to generate (provided as an argument)
num_programs="$1"

# Run gen.sh to generate programs
./gen.sh "$num_programs"

# Run the test script
./test.sh

# Prompt the user to decide whether to keep the generated programs
read -p "Do you want to keep the generated programs? [Y/n] " response

if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
  echo "Generated programs saved."
else
  # Delete the progs directory and its contents
  rm -rf progs
  echo "Generated programs deleted."
fi
