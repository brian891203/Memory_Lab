#!/bin/bash

# Set variables
GCC_PATH="/opt/msp430-gcc-9.3.1.2"
MSPDEBUG_BIN="/usr/local/bin/mspdebug"

# Check if MSP430 GCC tool is installed
if [ ! -d "$GCC_PATH" ]; then
    echo "Error: MSP430 GCC toolchain not installed at $GCC_PATH"
    exit 1
else
    echo "MSP430 GCC toolchain found at $GCC_PATH"
fi

# Check if MSPDebug is installed
if [ ! -f "$MSPDEBUG_BIN" ]; then
    echo "MSPDebug not found at $MSPDEBUG_BIN"
    echo "Attempting to install MSPDebug..."

    # Clone MSPDebug from GitHub
    git clone https://github.com/dlbeer/mspdebug.git
    cd mspdebug || { echo "Failed to clone MSPDebug repository."; exit 1; }

    # Install dependencies
    sudo apt update
    sudo apt install -y build-essential libusb-1.0-0-dev libreadline-dev

    # Build and install MSPDebug
    make
    sudo make install

    # Verify installation
    if [ ! -f "/usr/local/bin/mspdebug" ]; then
        echo "Failed to install MSPDebug."
        exit 1
    fi

    echo "MSPDebug installed successfully."

else
    echo "MSPDebug found at $MSPDEBUG_BIN"
fi

# Display MSPDebug version
echo "MSPDebug version:"
$MSPDEBUG_BIN --version

# Confirm environment setup
echo "Development environment setup is complete."
exit 0
