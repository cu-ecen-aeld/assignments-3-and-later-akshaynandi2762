#!/bin/sh

# Check that the required two arguments were provided
if [ $# -ne 2 ]; then
    echo "Error: Two parameters required to execute this function."
    echo "Usage: $0 <filesdir> <searchstr>"
    exit 1
fi

filesdir=$1
searchstr=$2

# Check that the directory exists
if [ ! -d "$filesdir" ]; then
    echo "Error: Directory '$filesdir' does not exist."
    exit 1
fi

# Count the number of files recursively
file_count=$(find "$filesdir" -type f | wc -l)

# Count matching lines
match_count=$(grep -R "$searchstr" "$filesdir" | wc -l)

# Output result
echo "The number of files are $file_count and the number of matching lines are $match_count"

exit 0

