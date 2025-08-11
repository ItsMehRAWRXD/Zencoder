#!/bin/bash

# Zencoder Build Script
# This script builds the Zencoder project with proper configuration

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    print_error "CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
fi

# Default build type
BUILD_TYPE="Release"
BUILD_DIR="build"
CLEAN_BUILD=false
INSTALL=false
INSTALL_PREFIX=""

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --install)
            INSTALL=true
            shift
            ;;
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        --help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --debug      Build in Debug mode (default: Release)"
            echo "  --clean      Clean build directory before building"
            echo "  --install    Install after building"
            echo "  --prefix DIR Set install prefix"
            echo "  --help       Show this help message"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

print_status "Building Zencoder in $BUILD_TYPE mode"

# Check for required dependencies
print_status "Checking dependencies..."

# Check for CMake
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Please install CMake (version 3.16 or higher)."
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
CMAKE_MAJOR=$(echo $CMAKE_VERSION | cut -d'.' -f1)
CMAKE_MINOR=$(echo $CMAKE_VERSION | cut -d'.' -f2)

if [ "$CMAKE_MAJOR" -lt 3 ] || ([ "$CMAKE_MAJOR" -eq 3 ] && [ "$CMAKE_MINOR" -lt 16 ]); then
    print_error "CMake version $CMAKE_VERSION is too old. Version 3.16 or higher is required."
    exit 1
fi

print_success "CMake $CMAKE_VERSION found"

# Check for C++ compiler
if command -v g++ &> /dev/null; then
    COMPILER="g++"
    COMPILER_VERSION=$(g++ --version | head -n1)
elif command -v clang++ &> /dev/null; then
    COMPILER="clang++"
    COMPILER_VERSION=$(clang++ --version | head -n1)
else
    print_error "No C++ compiler found. Please install g++ or clang++."
    exit 1
fi

print_success "C++ compiler found: $COMPILER_VERSION"

# Check for OpenSSL
if ! pkg-config --exists openssl; then
    print_warning "OpenSSL not found via pkg-config. Trying to find it manually..."
    if [ ! -f "/usr/include/openssl/ssl.h" ] && [ ! -f "/usr/local/include/openssl/ssl.h" ]; then
        print_error "OpenSSL development headers not found. Please install libssl-dev (Ubuntu/Debian) or openssl-devel (CentOS/RHEL)."
        exit 1
    fi
else
    OPENSSL_VERSION=$(pkg-config --modversion openssl)
    print_success "OpenSSL $OPENSSL_VERSION found"
fi

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    print_status "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
print_status "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
print_status "Configuring with CMake..."
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ -n "$INSTALL_PREFIX" ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
fi

cmake .. $CMAKE_ARGS

# Build the project
print_status "Building project..."
if command -v nproc &> /dev/null; then
    CORES=$(nproc)
else
    CORES=4  # Default fallback
fi

make -j$CORES

print_success "Build completed successfully!"

# Show build results
if [ -f "bin/zencoder" ]; then
    print_success "Executable created: bin/zencoder"
    ls -lh bin/zencoder
else
    print_warning "Executable not found in expected location"
fi

# Install if requested
if [ "$INSTALL" = true ]; then
    print_status "Installing..."
    if [ -n "$INSTALL_PREFIX" ]; then
        make install DESTDIR="$INSTALL_PREFIX"
        print_success "Installed to: $INSTALL_PREFIX"
    else
        sudo make install
        print_success "Installed system-wide"
    fi
fi

print_success "Build process completed!"
print_status "You can now run the application with: ./bin/zencoder"