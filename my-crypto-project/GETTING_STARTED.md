# Getting Started with My Crypto Project

This guide will help you get started with using the Zencoder base for your new crypto project.

## Quick Start

### 1. Project Structure Overview

```
my-crypto-project/
├── encryption/          # Encryption algorithm implementations
├── src/                 # Core source files
├── examples/            # Example implementations
├── simple_main.cpp      # Simplified entry point
├── main.cpp            # Original entry point
├── menu.cpp            # Menu system
└── packer.cpp          # File packing utilities
```

### 2. Choose Your Starting Point

You have two options:

#### Option A: Use the Simplified Version (Recommended for beginners)
- Start with `simple_main.cpp` - a clean, easy-to-understand menu system
- Gradually add features as needed
- Perfect for learning the codebase

#### Option B: Use the Full Version
- Use the original `main.cpp` with all features
- More complex but feature-complete
- Good if you need all encryption options immediately

### 3. First Steps

1. **Review the encryption algorithms** in the `encryption/` directory
2. **Check the example** in `examples/simple_encryption.cpp`
3. **Choose which algorithms** you want to keep
4. **Customize the menu** for your needs

### 4. Building Your First Feature

Here's a simple example to add file encryption to the simplified version:

```cpp
// In simple_main.cpp, replace the handleFileEncryption() function:

void handleFileEncryption() {
    std::cout << "\n=== File Encryption ===\n";
    
    // Get user input
    std::string inputFile, outputFile, password;
    
    std::cout << "Enter file to encrypt: ";
    std::getline(std::cin, inputFile);
    
    std::cout << "Enter output file name: ";
    std::getline(std::cin, outputFile);
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    // Add your encryption logic here
    // Example: Use AES-CBC from encryption/aes_cbc.cpp
    
    std::cout << "File encrypted successfully!\n";
}
```

### 5. Adding New Features

To add a new feature:

1. **Create a new function** in your main file
2. **Add a menu option** for it
3. **Implement the logic** using existing encryption algorithms
4. **Test thoroughly**

### 6. Customization Ideas

- **Simplify the encryption options**: Remove algorithms you don't need
- **Add a GUI**: Consider adding a graphical interface
- **Add network features**: Encrypt files over network
- **Create a library**: Turn it into a reusable encryption library
- **Add compression**: Combine encryption with compression

### 7. Development Tips

1. **Start small**: Get one feature working before adding more
2. **Test often**: Create test files to verify encryption/decryption
3. **Handle errors**: Add proper error handling for file operations
4. **Secure practices**: 
   - Never hardcode passwords
   - Use secure random number generation
   - Clear sensitive data from memory

### 8. Next Steps

1. **Rename the project files** to match your project name
2. **Update the Visual Studio solution** file
3. **Remove unused code** to keep it clean
4. **Add your own features**
5. **Create proper documentation**

## Example: Building a Simple File Encryptor

Here's a complete example you can build on:

```cpp
// simple_file_encryptor.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class FileEncryptor {
public:
    bool encryptFile(const std::string& inputPath, 
                    const std::string& outputPath,
                    const std::string& password) {
        // Your encryption implementation
        return true;
    }
    
    bool decryptFile(const std::string& inputPath,
                    const std::string& outputPath,
                    const std::string& password) {
        // Your decryption implementation
        return true;
    }
};

int main() {
    FileEncryptor encryptor;
    
    // Use the encryptor
    if (encryptor.encryptFile("input.txt", "output.enc", "mypassword")) {
        std::cout << "Encryption successful!" << std::endl;
    }
    
    return 0;
}
```

## Need Help?

- Check the original Zencoder documentation
- Look at the encryption algorithm implementations
- Start with the examples directory
- Build incrementally

Good luck with your crypto project! 🔐