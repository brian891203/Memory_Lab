#!/bin/bash

# Set variables
PROJECT_DIR="src"
MAKEFILE_PATH="$PROJECT_DIR/Makefile"

# Navigate to the project directory
if [ ! -d "$PROJECT_DIR" ]; then
    echo "Error: Project directory $PROJECT_DIR does not exist."
    exit 1
fi
cd "$PROJECT_DIR" || { echo "Unable to access project directory $PROJECT_DIR"; exit 1; }

# Check if Makefile exists
if [ ! -f "Makefile" ]; then
    echo "Error: Makefile not found in $PROJECT_DIR"
    exit 1
fi

# Clean up previous build
echo "Cleaning up old build files..."
make clean

# Build the project
echo "Compiling the code..."
make all

if [ $? -ne 0 ]; then
    echo "Compilation failed, please check the error messages."
    exit 1
fi

# Flash the program
echo "Flashing the program to MSP430FR4133..."
make flash

if [ $? -ne 0 ]; then
    echo "Flashing failed, please check if the MSP430 is properly connected."
    exit 1
fi

echo "Flashing completed successfully!"
exit 0
