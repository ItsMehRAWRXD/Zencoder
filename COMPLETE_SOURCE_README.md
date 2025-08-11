# Zencoder Complete Source Package

This is the complete source code for the Zencoder project, including all the features you requested:

## 🚀 Features Included

### ✅ Core Functionality
- **AES-CBC Encryption/Decryption** - Full implementation using OpenSSL
- **File Utilities** - Comprehensive file operations and management
- **Logging System** - Built-in logging with different levels
- **Interactive Menu System** - User-friendly command-line interface

### ✅ New Features (Your Requests)
- **Unlimited Stub Generator** - Generate executable stubs with various configurations
- **Drag and Drop Interface** - Windows-specific drag and drop functionality
- **File Attachment System** - Attach files to generated stubs and save as executables

### ✅ Build System
- **Cross-Platform CMake** - Works on Windows, Linux, and macOS
- **Visual Studio 2022 Support** - Full integration with Windows development
- **OpenSSL Integration** - Automatic dependency management
- **Testing Framework** - Built-in test suite

## 📁 Project Structure

```
zencoder/
├── src/                          # Main source code
│   ├── main.cpp                  # Entry point
│   ├── menu.cpp/h                # Interactive menu system
│   ├── packer.cpp/h              # File processing utilities
│   ├── encryption/               # Encryption modules
│   │   └── aes_cbc.cpp/h         # AES-CBC implementation
│   ├── utils/                    # Utility functions
│   │   ├── file_utils.cpp/h      # File operations
│   │   └── logging.cpp/h         # Logging system
│   ├── stub_generator.cpp/h      # 🆕 Stub generator
│   └── drag_drop.cpp/h           # 🆕 Drag and drop interface
├── include/                      # Public headers
├── test/                         # Test files
├── examples/                     # Example usage
├── types/                        # Type definitions
├── CMakeLists.txt                # Build configuration
├── build.sh                      # Linux/macOS build script
├── build.bat                     # Windows build script
└── setup_and_build.sh            # 🆕 Quick setup script
```

## 🛠️ Quick Start

### Option 1: Automatic Setup (Recommended)
```bash
# Make the setup script executable
chmod +x setup_and_build.sh

# Run the setup script
./setup_and_build.sh
```

### Option 2: Manual Build

#### Linux/macOS:
```bash
# Install dependencies
sudo apt install cmake libssl-dev build-essential  # Ubuntu/Debian
brew install cmake openssl                         # macOS

# Build the project
./build.sh
```

#### Windows (Visual Studio 2022):
```cmd
# Run the Windows build script
build.bat

# Or use CMake directly
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

## 🎯 New Features Usage

### Stub Generator
The stub generator allows you to create executable stubs with various features:

1. **Basic Stub** - Simple executable with encryption
2. **Self-Extracting Stub** - Extracts embedded files when run
3. **File Attachment** - Attach any file to a stub and save as executable
4. **Anti-Analysis Stub** - Includes anti-debug and anti-VM features

### Drag and Drop Interface
Windows-specific drag and drop functionality:

1. **Initialize** - Set up drag and drop for a window
2. **Enable/Disable** - Control drag and drop behavior
3. **Callback System** - Handle dropped files programmatically

## 🔧 Configuration

### Stub Configuration
```cpp
StubConfig config;
config.encryptionEnabled = true;    // Enable AES encryption
config.antiDebug = true;           // Include anti-debug code
config.antiVM = true;              // Include anti-VM code
config.persistence = false;        // Include persistence code
```

### Build Configuration
- **C++17 Standard** - Modern C++ features
- **OpenSSL Integration** - Cryptographic operations
- **Windows API** - Native Windows functionality
- **Cross-Platform** - Works on multiple operating systems

## 📋 Requirements

### System Requirements
- **C++17 Compatible Compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.16+**
- **OpenSSL Development Libraries**

### Windows Specific
- **Visual Studio 2022** (Community Edition works)
- **Windows SDK 10.0.19041.0 or later**
- **OpenSSL for Windows** (automatically detected)

## 🧪 Testing

Run the test suite to verify functionality:
```bash
# Linux/macOS
./build/zencoder_test

# Windows
build\zencoder_test.exe
```

## 📖 Examples

See the `examples/` directory for usage examples:
```bash
# Run the basic usage example
./build/zencoder_example
```

## 🔒 Security Features

- **AES-256-CBC Encryption** - Industry-standard encryption
- **Secure File Deletion** - Multi-pass secure deletion
- **Anti-Analysis Techniques** - Anti-debug and anti-VM capabilities
- **Memory Protection** - Secure memory handling

## 🐛 Troubleshooting

### Common Issues

1. **OpenSSL Not Found**
   ```bash
   # Ubuntu/Debian
   sudo apt install libssl-dev
   
   # Windows - Install via vcpkg or download from openssl.org
   ```

2. **CMake Not Found**
   ```bash
   # Ubuntu/Debian
   sudo apt install cmake
   
   # macOS
   brew install cmake
   ```

3. **Build Errors**
   - Ensure you have a C++17 compatible compiler
   - Check that all dependencies are installed
   - Verify CMake version is 3.16 or higher

### Getting Help

1. Check the build output for specific error messages
2. Verify all dependencies are correctly installed
3. Ensure you're using a supported compiler version
4. Check the `WINDOWS_SETUP.md` for detailed Windows instructions

## 📄 License

This project is licensed under the MIT License - see the `LICENSE` file for details.

## 🙏 Acknowledgments

- OpenSSL for cryptographic functionality
- CMake for cross-platform build system
- Windows API for native Windows features

---

**Ready to use!** The complete source package includes everything you need to build and run the Zencoder project with all the requested features.