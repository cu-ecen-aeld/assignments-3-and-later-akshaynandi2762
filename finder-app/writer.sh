#!/bin/sh

# Check that exactly two arguments were provided
if [ $# -ne 2 ]; then
    echo "Error: Two parameters required to execute this function."
    echo "Usage: $0 <writefile> <writestr>"
    exit 1
fi

writefile=$1
writestr=$2

# Extract the directory path from the full file path
writedir=$(dirname "$writefile")

# Create the directory path if it does not exist
mkdir -p "$writedir"
if [ $? -ne 0 ]; then
    echo "Error: Failed to create directory path '$writedir'."
    exit 1
fi

# Create or overwrite the file with the provided content
echo "$writestr" > "$writefile"
if [ $? -ne 0 ]; then
    echo "Error: Could not write to file '$writefile'."
    exit 1
fi

exit 0

