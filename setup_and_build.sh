#!/bin/bash

echo "=== Zencoder Complete Source Setup ==="
echo "This script will help you set up and build the Zencoder project."

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring project with CMake..."
if command -v cmake &> /dev/null; then
    cmake ..
    if [ $? -eq 0 ]; then
        echo "CMake configuration successful!"
        
        # Build the project
        echo "Building project..."
        make -j$(nproc)
        if [ $? -eq 0 ]; then
            echo "Build successful!"
            echo ""
            echo "=== Build Results ==="
            echo "Main executable: build/zencoder"
            echo "Test executable: build/zencoder_test"
            echo "Example executable: build/zencoder_example"
            echo ""
            echo "=== Usage ==="
            echo "Run the main program: ./build/zencoder"
            echo "Run tests: ./build/zencoder_test"
            echo "Run example: ./build/zencoder_example"
        else
            echo "Build failed. Please check the error messages above."
            exit 1
        fi
    else
        echo "CMake configuration failed. Please check the error messages above."
        exit 1
    fi
else
    echo "Error: CMake not found. Please install CMake first."
    echo "On Ubuntu/Debian: sudo apt install cmake"
    echo "On macOS: brew install cmake"
    echo "On Windows: Download from https://cmake.org/download/"
    exit 1
fi