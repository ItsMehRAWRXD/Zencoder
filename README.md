# Zencoder - Advanced Encryption and File Packing Tool

A comprehensive C++ utility for file encryption, decryption, and packing with support for multiple cryptographic algorithms.

## Features

### 🔐 Encryption Algorithms
- **AES-CBC** - Advanced Encryption Standard in Cipher Block Chaining mode
- **AES-CTR** - AES in Counter mode for parallel processing
- **ChaCha20** - High-performance stream cipher
- **Blowfish** - Fast block cipher
- **Twofish** - AES finalist with 128-bit block size
- **Serpent** - AES finalist with high security margin
- **Camellia** - Japanese block cipher
- **GCM** - Galois/Counter Mode for authenticated encryption
- **Poly1305** - High-speed message authentication code
- **RC4** - Stream cipher (legacy support)

### 📦 File Operations
- **Single File Encryption/Decryption**
- **Batch Folder Processing**
- **File Packing** - Combine multiple files into encrypted archives
- **PE File Manipulation** - Windows executable file handling
- **Memory Protection** - Kernel-level memory region protection

### 🛡️ Security Features
- **Cryptographically Secure Random Number Generation**
- **Memory Protection Hooks** (Windows)
- **Secure Key Material Handling**
- **Multiple Encryption Modes**

## Prerequisites

### Required Dependencies
- **CMake** (version 3.16 or higher)
- **C++17** compatible compiler
- **OpenSSL** development libraries

### Installing Dependencies

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libssl-dev
```

#### CentOS/RHEL/Fedora
```bash
sudo yum install gcc-c++ cmake openssl-devel
# or for newer versions:
sudo dnf install gcc-c++ cmake openssl-devel
```

#### macOS
```bash
brew install cmake openssl
```

#### Windows
- Install Visual Studio 2019 or later with C++ support
- Install CMake from https://cmake.org/
- Install OpenSSL from https://slproweb.com/products/Win32OpenSSL.html

## Building the Project

### Clone the Repository
```bash
git clone https://github.com/yourusername/zencoder.git
cd zencoder
```

### Build Instructions
```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
make -j$(nproc)  # Linux/macOS
# or
cmake --build . --config Release  # Windows
```

### Install (Optional)
```bash
# Install to system
sudo make install

# Or install to custom directory
make install DESTDIR=/path/to/install
```

## Usage

### Running the Application
```bash
# From build directory
./bin/zencoder

# Or if installed system-wide
zencoder
```

### Command Line Interface

The application provides an interactive menu system:

1. **Encryption/Decryption Operations**
   - Encrypt single files
   - Decrypt files
   - Batch folder encryption/decryption

2. **File Packing Operations**
   - Create encrypted archives
   - Extract from archives
   - List archive contents

3. **PE File Manipulation**
   - Analyze Windows executables
   - Modify PE headers
   - Add custom sections

4. **Security and Protection Options**
   - Memory region protection
   - Kernel hook management
   - Security monitoring

### Example Workflow

1. **Encrypt a File**
   ```
   Select: 1 (Encryption/Decryption)
   Select: 1 (Encrypt a file)
   Choose algorithm: AES-CBC
   Enter input file: /path/to/file.txt
   Enter output file: /path/to/file.enc
   ```

2. **Create Encrypted Archive**
   ```
   Select: 2 (File Packing)
   Select: 1 (Create archive)
   Choose algorithm: ChaCha20
   Enter files: file1.txt, file2.txt, file3.txt
   Enter output: archive.z3
   ```

## Project Structure

```
zencoder/
├── CMakeLists.txt          # Build configuration
├── README.md              # This file
├── include/               # Public headers
│   └── packer.h          # Main API header
├── src/                   # Source code
│   ├── main.cpp          # Application entry point
│   ├── menu.cpp          # User interface
│   ├── menu.h            # Menu declarations
│   ├── packer.cpp        # Core packing logic
│   ├── packer.h          # Packer declarations
│   ├── encryption/       # Encryption algorithms
│   │   ├── aes_cbc.cpp
│   │   ├── aes_ctr.cpp
│   │   ├── chacha20.cpp
│   │   └── ...
│   └── utils/            # Utility functions
│       ├── file_utils.cpp
│       └── logging.cpp
├── types/                # Type definitions
│   └── enums.h          # Enumeration types
├── test/                 # Test files
└── driver/              # Driver components
```

## Security Considerations

### ⚠️ Important Notes
- This tool is for educational and legitimate security research purposes
- Always backup your files before encryption
- Store encryption keys securely
- The tool includes memory protection features but should not be considered a complete security solution

### 🔒 Security Features
- **Secure Random Generation**: Uses hardware entropy sources
- **Memory Protection**: Kernel-level memory region protection (Windows)
- **Key Material Handling**: Secure wiping of sensitive data
- **Algorithm Validation**: Proper implementation of cryptographic standards

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow C++17 standards
- Use meaningful variable names
- Add proper error handling
- Include unit tests for new features
- Document all public APIs

## Testing

### Running Tests
```bash
cd build
make test
```

### Manual Testing
```bash
# Test file encryption
echo "Hello, World!" > test.txt
./bin/zencoder
# Use menu to encrypt test.txt
# Verify decryption works correctly
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- OpenSSL project for cryptographic primitives
- Contributors and security researchers
- Open source community

## Support

For issues and questions:
- Create an issue on GitHub
- Check the documentation
- Review the source code

---

**Disclaimer**: This tool is provided as-is for educational purposes. Users are responsible for ensuring compliance with local laws and regulations regarding cryptography and data protection.