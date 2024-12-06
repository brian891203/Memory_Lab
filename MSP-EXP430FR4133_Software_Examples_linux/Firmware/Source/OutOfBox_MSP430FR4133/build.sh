#!/bin/bash

# Exit script on any error
set -e

# Clean up previous build
echo "Cleaning up old build files..."
make clean

# Build the project
echo "Building the project using Makefile..."
make

# Flash the program to the MSP430
echo "Flashing main.elf to the MSP430..."
/usr/local/bin/mspdebug ezfet "prog main.elf"

# Optional cleanup
# Uncomment the line below to clean build files after flashing
# make clean

echo "Build and flash completed successfully!"
