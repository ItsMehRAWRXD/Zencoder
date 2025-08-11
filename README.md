#include <iostream>
#include "menu.h"

int main() {
    // Initialize the application
    std::cout << "Welcome to the Encryption and Packing Tool!" << std::endl;

    // Display the menu and handle user input
    while (true) {
        int choice = displayMenu();
        if (choice == 0) {
            break; // Exit the application
        }
        handleUserChoice(choice);
    }

    return 0;
}
```

**src/menu.cpp**
```cpp
#include <iostream>
#include "menu.h"

int displayMenu() {
    std::cout << "Select an option:" << std::endl;
    std::cout << "1. AES-CBC Encryption" << std::endl;
    std::cout << "2. AES-CTR Encryption" << std::endl;
    std::cout << "3. Blowfish Encryption" << std::endl;
    std::cout << "4. Camellia Encryption" << std::endl;
    std::cout << "5. ChaCha20 Encryption" << std::endl;
    std::cout << "6. GCM Encryption" << std::endl;
    std::cout << "7. Poly1305 Authentication" << std::endl;
    std::cout << "8. RC4 Encryption" << std::endl;
    std::cout << "9. Serpent Encryption" << std::endl;
    std::cout << "10. Twofish Encryption" << std::endl;
    std::cout << "11. Repack Files" << std::endl;
    std::cout << "0. Exit" << std::endl;

    int choice;
    std::cin >> choice;
    return choice;
}

void handleUserChoice(int choice) {
    switch (choice) {
        case 1:
            // Call AES-CBC encryption function
            break;
        case 2:
            // Call AES-CTR encryption function
            break;
        case 3:
            // Call Blowfish encryption function
            break;
        case 4:
            // Call Camellia encryption function
            break;
        case 5:
            // Call ChaCha20 encryption function
            break;
        case 6:
            // Call GCM encryption function
            break;
        case 7:
            // Call Poly1305 authentication function
            break;
        case 8:
            // Call RC4 encryption function
            break;
        case 9:
            // Call Serpent encryption function
            break;
        case 10:
            // Call Twofish encryption function
            break;
        case 11:
            // Call repack files function
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
    }
}
```

**src/menu.h**
```cpp
#ifndef MENU_H
#define MENU_H

int displayMenu();
void handleUserChoice(int choice);

#endif // MENU_H
```

**src/encryption/aes_cbc.cpp**
```cpp
#include "aes_cbc.h"

// Implementation of AES-CBC encryption algorithm
```

**src/encryption/aes_cbc.h**
```cpp
#ifndef AES_CBC_H
#define AES_CBC_H

// Declarations for AES-CBC encryption functions

#endif // AES_CBC_H
```

**src/encryption/aes_ctr.cpp**
```cpp
#include "aes_ctr.h"

// Implementation of AES-CTR encryption algorithm
```

**src/encryption/aes_ctr.h**
```cpp
#ifndef AES_CTR_H
#define AES_CTR_H

// Declarations for AES-CTR encryption functions

#endif // AES_CTR_H
```

**src/encryption/blowfish.cpp**
```cpp
#include "blowfish.h"

// Implementation of Blowfish encryption algorithm
```

**src/encryption/blowfish.h**
```cpp
#ifndef BLOWFISH_H
#define BLOWFISH_H

// Declarations for Blowfish encryption functions

#endif // BLOWFISH_H
```

**src/encryption/camellia.cpp**
```cpp
#include "camellia.h"

// Implementation of Camellia encryption algorithm
```

**src/encryption/camellia.h**
```cpp
#ifndef CAMELLIA_H
#define CAMELLIA_H

// Declarations for Camellia encryption functions

#endif // CAMELLIA_H
```

**src/encryption/chacha20.cpp**
```cpp
#include "chacha20.h"

// Implementation of ChaCha20 encryption algorithm
```

**src/encryption/chacha20.h**
```cpp
#ifndef CHACHA20_H
#define CHACHA20_H

// Declarations for ChaCha20 encryption functions

#endif // CHACHA20_H
```

**src/encryption/encryption_base.cpp**
```cpp
#include "encryption_base.h"

// Implementation of base class for encryption algorithms
```

**src/encryption/encryption_base.h**
```cpp
#ifndef ENCRYPTION_BASE_H
#define ENCRYPTION_BASE_H

// Declarations for base class for encryption algorithms

#endif // ENCRYPTION_BASE_H
```

**src/encryption/gcm.cpp**
```cpp
#include "gcm.h"

// Implementation of GCM encryption algorithm
```

**src/encryption/gcm.h**
```cpp
#ifndef GCM_H
#define GCM_H

// Declarations for GCM encryption functions

#endif // GCM_H
```

**src/encryption/poly1305.cpp**
```cpp
#include "poly1305.h"

// Implementation of Poly1305 authentication algorithm
```

**src/encryption/poly1305.h**
```cpp
#ifndef POLY1305_H
#define POLY1305_H

// Declarations for Poly1305 authentication functions

#endif // POLY1305_H
```

**src/encryption/rc4.cpp**
```cpp
#include "rc4.h"

// Implementation of RC4 encryption algorithm
```

**src/encryption/rc4.h**
```cpp
#ifndef RC4_H
#define RC4_H

// Declarations for RC4 encryption functions

#endif // RC4_H
```

**src/encryption/serpent.cpp**
```cpp
#include "serpent.h"

// Implementation of Serpent encryption algorithm
```

**src/encryption/serpent.h**
```cpp
#ifndef SERPENT_H
#define SERPENT_H

// Declarations for Serpent encryption functions

#endif // SERPENT_H
```

**src/encryption/twofish.cpp**
```cpp
#include "twofish.h"

// Implementation of Twofish encryption algorithm
```

**src/encryption/twofish.h**
```cpp
#ifndef TWOFISH_H
#define TWOFISH_H

// Declarations for Twofish encryption functions

#endif // TWOFISH_H
```

**src/utils/file_utils.cpp**
```cpp
#include "file_utils.h"

// Implementation of utility functions for file operations
```

**src/utils/file_utils.h**
```cpp
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

// Declarations for utility functions for file operations

#endif // FILE_UTILS_H
```

**src/utils/logging.cpp**
```cpp
#include "logging.h"

// Implementation of logging functionality
```

**src/utils/logging.h**
```cpp
#ifndef LOGGING_H
#define LOGGING_H

// Declarations for logging functions

#endif // LOGGING_H
```

**src/types/enums.h**
```cpp
#ifndef ENUMS_H
#define ENUMS_H

// Enumeration definitions for encryption types

#endif // ENUMS_H
```

**CMakeLists.txt**
```cmake
cmake_minimum_required(VERSION 3.10)
project(EncryptionPacker)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

file(GLOB SOURCES "src/*.cpp" "src/encryption/*.cpp" "src/utils/*.cpp" "src/types/*.cpp")

add_executable(EncryptionPacker ${SOURCES})
```

**.gitignore**
```
build/
*.o
*.exe
*.out
*.old
```

**README.md**
```
# Encryption and Packing Tool

## Description
This project provides a tool for encrypting files using various algorithms and repacking them.

## Building the Project
To build the project, use the following commands:
```
mkdir build
cd build
cmake ..
make
```

## Usage
Run the executable and follow the on-screen instructions to select encryption methods and repack files.
```

This structure provides a complete framework for your project, allowing for the selection of various encryption methods and repacking functionality.