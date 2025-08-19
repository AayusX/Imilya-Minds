#!/bin/bash

# Build and Run Script for C++ Search App
echo "Building C++ Search App..."

# Ensure we are in the project directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || { echo "Failed to cd to script directory"; exit 1; }

# Create build directory if it doesn't exist
mkdir -p build
cd build || { echo "Failed to cd to build directory"; exit 1; }

# Configure with CMake
echo "Configuring with CMake..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

# Build the application
echo "Building application..."
make -j$(nproc) || { echo "Build failed"; exit 1; }

echo "Build successful! Running application..."
echo "----------------------------------------"

# Run the application (skip if no display server)
if [ -z "$DISPLAY" ] && [ -z "$WAYLAND_DISPLAY" ]; then
  echo "No display detected (headless). Skipping app launch. Binary is at: $(pwd)/bin/CPPSearchApp"
  exit 0
fi

./bin/CPPSearchApp
