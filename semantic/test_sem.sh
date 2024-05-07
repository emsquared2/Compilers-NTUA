#!/bin/bash

# Counters for correct and erroneous program results
correct_successes=0
erroneous_failures=0

# Arrays to track failed correct and succeeded erroneous programs
declare -a correct_failures
declare -a erroneous_successes

# Paths to the directories and executable
correct_dir="../programs/"
erroneous_dir="../programs-erroneous/"
grace_executable="../semantic/grace"

# Total file counts
correct_total=0
erroneous_total=0

# Check each file in the correct programs directory
for correct_file in "${correct_dir}"*; do
    ((correct_total++))

    # Run the grace executable with this file
    result=$( "$grace_executable" < "$correct_file" | tr -d '[:space:]' )

    # Check if the result matches the expected "Success."
    if [[ "$result" == "Success." ]]; then
        ((correct_successes++))
    else
        correct_failures+=("$(basename "$correct_file")")
    fi
done

# Check each file in the erroneous programs directory
for erroneous_file in "${erroneous_dir}"*; do
    ((erroneous_total++))

    # Run the grace executable with this file
    result=$( "$grace_executable" < "$erroneous_file" | tr -d '[:space:]' )

    # Check that the result does not match "Success."
    if [[ "$result" != "Success." ]]; then
        ((erroneous_failures++))
    else
        erroneous_successes+=("$(basename "$erroneous_file")")
    fi
done

echo "--------------------------"

# Print results for correct programs
echo "$correct_successes / $correct_total correct programs succeeded."

if [ ${#correct_failures[@]} -ne 0 ]; then
    echo "The following correct programs failed:"
    for failure in "${correct_failures[@]}"; do
        echo "$failure"
    done
fi

# Print results for erroneous programs
echo "$erroneous_failures / $erroneous_total erroneous programs failed."

if [ ${#erroneous_successes[@]} -ne 0 ]; then
    echo "The following erroneous programs succeeded:"
    for success in "${erroneous_successes[@]}"; do
        echo "$success"
    done
fi
