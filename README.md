# Zencoder

## Description
Zencoder is an advanced encryption and file packing utility that provides multiple cryptographic algorithms and PE file manipulation capabilities. The tool offers a comprehensive menu-driven interface for encrypting files, packing them into archives, and performing various security operations.

## Features

### Encryption Algorithms
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

### File Operations
- File and folder encryption/decryption
- Multi-file packing with encryption
- PE file manipulation and protection
- Memory protection and kernel hooks
- Secure key generation and management

## Building the Project

### Visual Studio 2022
1. Open `Zencoder.sln` in Visual Studio 2022
2. Select your desired configuration (Debug/Release) and platform (x86/x64)
3. Build the solution (Ctrl+Shift+B)

### Requirements
- Visual Studio 2022 with C++ toolset (v143)
- Windows SDK 10.0
- C++17 standard support

## Usage
Run the executable and follow the interactive menu:

```
==================================
      Advanced Crypto Utility     
==================================
This tool provides encryption, packing, and PE manipulation capabilities.

==== Encryption and Packing Tool ====
1. Encryption/Decryption Operations
2. File Packing Operations
3. PE File Manipulation
4. Security and Protection Options
0. Exit
```

Select the desired operation and follow the prompts to:
- Encrypt/decrypt individual files or folders
- Pack multiple files into encrypted archives
- Manipulate PE file structures
- Configure security and protection options

## Project Structure

```
Zencoder/
├── src/
│   ├── encryption/          # Encryption algorithm implementations
│   ├── utils/              # Utility functions
│   └── types/              # Type definitions
├── include/                # Header files
├── main.cpp               # Application entry point
├── menu.cpp               # Menu system implementation
├── packer.cpp             # File packing functionality
└── Zencoder.sln          # Visual Studio solution file
```

## Security Notice
This tool implements various cryptographic algorithms for educational and legitimate security purposes. Always ensure you have proper authorization before using encryption tools on systems or files you do not own.

## License
Please refer to the license file for usage terms and conditions.