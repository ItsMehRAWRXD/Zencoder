# My Crypto Project

## Description
This is a crypto utility project based on Zencoder. It provides a foundation for building encryption and file manipulation tools with various cryptographic algorithms.

## Features

### Available Encryption Algorithms
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

### Core Capabilities
- File and folder encryption/decryption
- Multi-file packing with encryption
- PE file manipulation (Windows)
- Secure key generation and management

## Getting Started

### Prerequisites
- Visual Studio 2022 with C++ toolset (v143)
- Windows SDK 10.0
- C++17 standard support

### Building the Project

#### Using Visual Studio
1. Open `Zencoder.sln` in Visual Studio 2022
2. Select your desired configuration (Debug/Release) and platform (x86/x64)
3. Build the solution (Ctrl+Shift+B)

#### Using Command Line (Windows)
```bash
# Build using MSBuild
msbuild Zencoder.sln /p:Configuration=Release /p:Platform=x64
```

## Project Structure

```
my-crypto-project/
├── src/                    # Source files
├── encryption/             # Encryption algorithm implementations
├── include/                # Header files
├── utils/                  # Utility functions
├── types/                  # Type definitions
├── main.cpp               # Application entry point
├── menu.cpp               # Menu system implementation
├── packer.cpp             # File packing functionality
└── Zencoder.sln          # Visual Studio solution file
```

## Customization Guide

### Adding New Features
1. **New Encryption Algorithm**: Add implementation in `encryption/` directory
2. **New Menu Option**: Modify `menu.cpp` and `menu.h`
3. **New Functionality**: Create new source files in `src/`

### Simplifying the Project
If you want a simpler version:
1. Remove unused encryption algorithms from `encryption/`
2. Simplify the menu system in `menu.cpp`
3. Focus on specific features you need

## Usage Example

```cpp
// Basic file encryption example
#include "encryption/aes_cbc.h"

int main() {
    // Your encryption logic here
    return 0;
}
```

## Next Steps
1. Rename the Visual Studio solution and project files to match your project name
2. Customize the menu options in `menu.cpp` for your specific use case
3. Remove any unnecessary features or encryption algorithms
4. Add your own custom functionality

## Security Notice
This tool implements cryptographic algorithms. Always ensure:
- Proper key management
- Secure random number generation
- Appropriate algorithm selection for your use case
- Compliance with relevant regulations

## License
[Add your license here]