#!/bin/bash

# Build the program using the Makefile
make clean
make

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Executing the program..."
    # Execute the binary from the bin folder
    ./bin/sim
else
    echo "Build failed. Please check for errors."
fi

