# Rawr - Advanced Encryption & File Processing Toolkit (Windows)

## Overview
Rawr is a powerful Windows application for file encryption, packing, and PE manipulation with advanced features including a polymorphic stub generator.

## Features
- **Multiple Encryption Algorithms**: AES-CBC, AES-CTR, ChaCha20, Blowfish, Twofish, Serpent, Camellia, RC4, XOR
- **File Operations**: Single/batch file encryption and decryption
- **PE File Manipulation**: Import/export table obfuscation, section injection
- **Polymorphic Stub Generator**: Create unique, self-modifying stubs with anti-analysis features
- **Multi-file Packing**: Bundle multiple files into encrypted containers
- **Security Features**: Anti-debug, anti-VM, memory protection

## Building on Windows

### Requirements
- Visual Studio 2022 (or later) with C++ development tools
- Windows SDK 10.0 or later
- C++17 compatible compiler

### Using Visual Studio
1. Open `Rawr.sln` in Visual Studio
2. Select your target configuration (Debug/Release, x86/x64)
3. Build the solution (Ctrl+Shift+B)

### Using CMake (Optional)
```cmd
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

## Usage

Run `Rawr.exe` to start the interactive menu:

```
==== Encryption and Packing Tool ====
1. Encryption/Decryption Operations
2. File Packing Operations
3. PE File Manipulation
4. Security and Protection Options
5. Polymorphic Stub Generator
0. Exit
```

### Polymorphic Stub Generator
The stub generator creates unique, self-modifying code that can:
- Generate different stub types (loader, injector, dropper, crypter)
- Add anti-debug and anti-VM protections
- Apply code obfuscation and randomization
- Attach payloads and build executables
- Batch generate unique stubs

## Project Structure
```
Rawr/
├── Rawr.sln              # Visual Studio solution
├── Rawr.vcxproj          # Visual Studio project
├── main.cpp              # Main entry point
├── menu.cpp/h            # Menu system
├── packer.cpp/h          # Core packing functionality
├── encryption/           # Encryption implementations
├── utils/                # Utility functions (PE parser, file ops)
├── types/                # Data types and structures
├── src/                  # Additional source files
│   ├── stub_generator.cpp/h  # Polymorphic stub generator
│   └── protect.cpp/h         # Protection mechanisms
└── include/              # Additional headers
```

## Notes
- The polymorphic stub generator creates Windows PE executables
- PE manipulation features are Windows-specific
- Anti-debug/anti-VM features are designed for Windows environments
- Some features may require administrator privileges

## License
This project is provided for educational and research purposes. Use responsibly and in accordance with applicable laws.