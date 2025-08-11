# Rawr - Advanced Encryption & File Processing Toolkit

## Description
Rawr is a powerful encryption and file manipulation toolkit based on the Zencoder framework. It provides multiple cryptographic algorithms, file packing capabilities, and PE file manipulation features for Windows.

## Features

### 🔐 Encryption Algorithms
- **AES-CBC** - Advanced Encryption Standard with Cipher Block Chaining
- **AES-CTR** - Advanced Encryption Standard with Counter mode
- **ChaCha20** - Modern stream cipher
- **Blowfish** - Symmetric block cipher
- **Twofish** - Symmetric key block cipher
- **Serpent** - Symmetric key block cipher
- **Camellia** - Block cipher algorithm
- **RC4** - Stream cipher
- **GCM** - Galois/Counter Mode authenticated encryption
- **Poly1305** - Cryptographic message authentication

### 📦 Core Capabilities
- File and folder encryption/decryption
- Multi-file packing with encryption
- PE file manipulation (Windows)
- Memory protection and kernel hooks
- Secure key generation and management

## Getting Started

### Prerequisites
- C++ compiler with C++17 support (g++, clang, MSVC)
- CMake 3.16 or higher (for CMake build)
- Visual Studio 2022 (for VS solution)
- Windows SDK 10.0 (for Windows features)

### Building with CMake

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run
./bin/rawr
```

### Building with Visual Studio
1. Open `Zencoder.sln` in Visual Studio 2022
2. Select configuration (Debug/Release) and platform (x86/x64)
3. Build solution (Ctrl+Shift+B)

### Building with g++ (Simple Example)
```bash
# Build the simple encryption example
g++ -std=c++17 examples/simple_encryption.cpp -o simple_encrypt

# Build the simplified main
g++ -std=c++17 simple_main.cpp -o rawr_simple
```

## Project Structure

```
Rawr/
├── driver/                 # Kernel driver components
├── encryption/             # Encryption algorithm implementations
├── include/                # Header files
├── src/                    # Additional source files
├── test/                   # Test files
├── types/                  # Type definitions
├── utils/                  # Utility functions
├── examples/               # Example implementations
│   └── simple_encryption.cpp
├── main.cpp               # Full-featured entry point
├── simple_main.cpp        # Simplified entry point
├── menu.cpp               # Menu system implementation
├── packer.cpp             # File packing functionality
├── CMakeLists.txt         # CMake build configuration
└── README.md              # This file
```

## Usage

### Option 1: Full Featured Application
```bash
./rawr
```
This launches the complete application with all features:
- Encryption/Decryption operations
- File packing operations
- PE file manipulation
- Security and protection options

### Option 2: Simplified Interface
```bash
./rawr_simple
```
A cleaner, simpler interface ideal for:
- Learning the codebase
- Basic encryption tasks
- Custom development base

### Option 3: Example Programs
```bash
./simple_encrypt
```
Standalone encryption example for quick file encryption/decryption.

## Development Guide

### Adding New Features
1. **New Encryption Algorithm**: Add implementation in `encryption/`
2. **New Menu Option**: Modify `menu.cpp` and `menu.h`
3. **New Functionality**: Create source files in `src/`

### Using with Augment (VS Code)
This project is perfect for use with the Augment AI coding assistant:
- Complex algorithms to explain
- TODO items to implement
- Refactoring opportunities
- Security improvements

See `AUGMENT_SETUP.md` for detailed Augment usage guide.

## Examples

### Basic File Encryption
```cpp
// Using the simple encryptor
SimpleEncryptor enc("mypassword");
auto encrypted = enc.encrypt(readFile("input.txt"));
writeFile("output.enc", encrypted);
```

### Using with Menu System
```cpp
// The menu system provides interactive options
// Run ./rawr and select:
// 1. Encryption/Decryption Operations
// 2. Choose your algorithm
// 3. Select files to process
```

## Security Notice
This toolkit implements cryptographic algorithms. Always ensure:
- Proper key management
- Secure random number generation  
- Appropriate algorithm selection
- Compliance with regulations
- Authorization for use on target systems

## Contributing
Feel free to contribute by:
- Adding new encryption algorithms
- Improving existing implementations
- Adding tests
- Enhancing documentation

## License
[Add your license information here]

---
Built with ❤️ for secure file processing and encryption needs.