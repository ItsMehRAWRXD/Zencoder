# Zencoder Project Structure

This document provides an overview of the Zencoder project structure and organization.

## Directory Layout

```
zencoder/
├── CMakeLists.txt              # Main build configuration
├── README.md                   # Project documentation
├── LICENSE                     # MIT License
├── .gitignore                  # Git ignore rules
├── build.sh                    # Build script
├── PROJECT_STRUCTURE.md        # This file
│
├── include/                    # Public headers
│   └── packer.h               # Main API header
│
├── src/                        # Source code
│   ├── main.cpp               # Application entry point
│   ├── menu.cpp               # User interface implementation
│   ├── menu.h                 # Menu system declarations
│   ├── packer.cpp             # Core packing logic
│   ├── packer.h               # Packer class declarations
│   │
│   ├── encryption/            # Encryption algorithms
│   │   ├── aes_cbc.cpp        # AES-CBC implementation
│   │   ├── aes_cbc.h          # AES-CBC header
│   │   ├── aes_ctr.cpp        # AES-CTR implementation
│   │   ├── aes_ctr.h          # AES-CTR header
│   │   ├── chacha20.cpp       # ChaCha20 implementation
│   │   ├── chacha20.h         # ChaCha20 header
│   │   ├── blowfish.cpp       # Blowfish implementation
│   │   ├── blowfish.h         # Blowfish header
│   │   ├── twofish.cpp        # Twofish implementation
│   │   ├── twofish.h          # Twofish header
│   │   ├── serpent.cpp        # Serpent implementation
│   │   ├── serpent.h          # Serpent header
│   │   ├── camellia.cpp       # Camellia implementation
│   │   ├── camellia.h         # Camellia header
│   │   ├── gcm.cpp            # GCM implementation
│   │   ├── gcm.h              # GCM header
│   │   ├── poly1305.cpp       # Poly1305 implementation
│   │   ├── poly1305.h         # Poly1305 header
│   │   ├── rc4.cpp            # RC4 implementation
│   │   └── rc4.h              # RC4 header
│   │
│   └── utils/                 # Utility functions
│       ├── file_utils.cpp     # File operations
│       ├── file_utils.h       # File utilities header
│       ├── logging.cpp        # Logging functionality
│       └── logging.h          # Logging header
│
├── types/                     # Type definitions
│   └── enums.h               # Enumeration types
│
├── test/                      # Test files
│   └── simple_test.cpp       # Basic functionality test
│
├── examples/                  # Usage examples
│   └── basic_usage.cpp       # Basic usage demonstration
│
└── driver/                    # Driver components (if any)
```

## Key Components

### 1. Core Application (`src/`)
- **main.cpp**: Application entry point with initialization
- **menu.cpp/h**: Interactive menu system for user operations
- **packer.cpp/h**: Core file packing and encryption logic

### 2. Encryption Module (`src/encryption/`)
Each encryption algorithm is implemented as a separate class with:
- **Header file**: Class declaration and public interface
- **Implementation file**: Algorithm implementation using OpenSSL

**Supported Algorithms:**
- AES-CBC (128/192/256-bit)
- AES-CTR (128/192/256-bit)
- ChaCha20
- Blowfish
- Twofish
- Serpent
- Camellia
- GCM (Galois/Counter Mode)
- Poly1305 (Message Authentication)
- RC4 (Legacy support)

### 3. Utilities (`src/utils/`)
- **file_utils.cpp/h**: Comprehensive file operations
- **logging.cpp/h**: Logging and debugging utilities

### 4. Type System (`types/`)
- **enums.h**: Type-safe enumerations for algorithms, operations, and status codes

### 5. Build System
- **CMakeLists.txt**: Modern CMake configuration
- **build.sh**: Convenient build script with dependency checking

### 6. Documentation
- **README.md**: Comprehensive project documentation
- **PROJECT_STRUCTURE.md**: This file
- **LICENSE**: MIT License

### 7. Testing and Examples
- **test/**: Unit tests and verification
- **examples/**: Usage examples and demonstrations

## Design Principles

### 1. Modularity
- Each encryption algorithm is self-contained
- Clear separation between UI, business logic, and utilities
- Easy to add new algorithms or modify existing ones

### 2. Security
- Secure random number generation
- Memory protection for sensitive data
- Proper key management
- Secure file deletion

### 3. Portability
- Cross-platform compatibility (Linux, Windows, macOS)
- Standard C++17 features
- Minimal external dependencies (OpenSSL only)

### 4. Usability
- Interactive menu system
- Comprehensive error handling
- Clear documentation and examples
- Easy build process

## File Naming Conventions

- **Headers**: `.h` extension
- **Implementation**: `.cpp` extension
- **Test files**: `*_test.cpp` or `test_*.cpp`
- **Example files**: `*_example.cpp` or `example_*.cpp`

## Build Output Structure

```
build/
├── bin/                       # Executables
│   └── zencoder              # Main application
├── lib/                       # Libraries (if any)
└── CMakeFiles/               # CMake generated files
```

## Dependencies

### Required
- **CMake** (3.16+)
- **C++17** compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **OpenSSL** development libraries

### Optional
- **pkg-config** (for dependency detection)
- **Doxygen** (for documentation generation)

## Development Workflow

1. **Setup**: Install dependencies and clone repository
2. **Build**: Use `./build.sh` or CMake directly
3. **Test**: Run tests to verify functionality
4. **Develop**: Add features or modify existing code
5. **Document**: Update documentation as needed

## Contributing Guidelines

1. Follow the existing code style
2. Add tests for new features
3. Update documentation
4. Use meaningful commit messages
5. Ensure cross-platform compatibility

## Security Considerations

- All cryptographic operations use OpenSSL
- Keys are generated using cryptographically secure random number generators
- Sensitive data is securely wiped from memory
- File operations include safety checks
- Input validation prevents path traversal attacks