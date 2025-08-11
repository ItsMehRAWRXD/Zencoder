#include "menu.h"
#include "../src/encryption/aes_cbc.h"
#include "../src/utils/file_utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

void displayMenu() {
    bool exitProgram = false;
    
    while (!exitProgram) {
        std::cout << "\n==== Zencoder Crypto Utility ====\n";
        std::cout << "1. Encryption/Decryption Operations\n";
        std::cout << "2. File Operations\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 0) {
            exitProgram = true;
        } else if (choice == 1) {
            displayEncryptionMenu();
            int encChoice;
            std::cin >> encChoice;
            handleEncryptionChoice(encChoice);
        } else if (choice == 2) {
            displayFileMenu();
            int fileChoice;
            std::cin >> fileChoice;
            handleFileChoice(fileChoice);
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    std::cout << "Thank you for using Zencoder!\n";
}

void displayEncryptionMenu() {
    std::cout << "\n==== Encryption/Decryption Operations ====\n";
    std::cout << "1. Encrypt a file (AES-CBC)\n";
    std::cout << "2. Decrypt a file (AES-CBC)\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handleEncryptionChoice(int choice) {
    switch (choice) {
        case 1: {
            std::cout << "Enter input file path: ";
            std::string inputFile = getInputPath("Input file");
            if (inputFile.empty()) return;
            
            std::cout << "Enter output file path: ";
            std::string outputFile = getInputPath("Output file");
            if (outputFile.empty()) return;
            
            // Read the input file
            auto fileData = FileUtils::readFile(inputFile);
            if (fileData.empty()) {
                std::cout << "Error: Could not read input file.\n";
                return;
            }
            
            // Initialize AES encryption
            AESCBC aes;
            auto key = AESCBC::generateKey(32);  // 256-bit key
            auto iv = AESCBC::generateIV();
            
            if (!aes.initialize(key, iv)) {
                std::cout << "Error: Failed to initialize AES encryption.\n";
                return;
            }
            
            // Encrypt the data
            auto encryptedData = aes.encrypt(fileData);
            if (encryptedData.empty()) {
                std::cout << "Error: Encryption failed.\n";
                return;
            }
            
            // Save encrypted data
            if (!FileUtils::writeFile(outputFile, encryptedData)) {
                std::cout << "Error: Failed to save encrypted file.\n";
                return;
            }
            
            std::cout << "File encrypted successfully: " << outputFile << "\n";
            break;
        }
        
        case 2: {
            std::cout << "Enter encrypted file path: ";
            std::string inputFile = getInputPath("Encrypted file");
            if (inputFile.empty()) return;
            
            std::cout << "Enter output file path: ";
            std::string outputFile = getInputPath("Output file");
            if (outputFile.empty()) return;
            
            // Read the encrypted file
            auto encryptedData = FileUtils::readFile(inputFile);
            if (encryptedData.empty()) {
                std::cout << "Error: Could not read encrypted file.\n";
                return;
            }
            
            // Initialize AES decryption
            AESCBC aes;
            auto key = AESCBC::generateKey(32);  // 256-bit key
            auto iv = AESCBC::generateIV();
            
            if (!aes.initialize(key, iv)) {
                std::cout << "Error: Failed to initialize AES decryption.\n";
                return;
            }
            
            // Decrypt the data
            auto decryptedData = aes.decrypt(encryptedData);
            if (decryptedData.empty()) {
                std::cout << "Error: Decryption failed.\n";
                return;
            }
            
            // Save decrypted data
            if (!FileUtils::writeFile(outputFile, decryptedData)) {
                std::cout << "Error: Failed to save decrypted file.\n";
                return;
            }
            
            std::cout << "File decrypted successfully: " << outputFile << "\n";
            break;
        }
        
        case 0:
            return;
            
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}

void displayFileMenu() {
    std::cout << "\n==== File Operations ====\n";
    std::cout << "1. Get file information\n";
    std::cout << "2. Create backup of file\n";
    std::cout << "3. Securely delete file\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handleFileChoice(int choice) {
    switch (choice) {
        case 1: {
            std::string filePath = getInputPath("File path");
            if (filePath.empty()) return;
            
            if (!FileUtils::fileExists(filePath)) {
                std::cout << "Error: File does not exist.\n";
                return;
            }
            
            std::cout << "File Information:\n";
            std::cout << "  Path: " << filePath << "\n";
            std::cout << "  Size: " << FileUtils::getFileSize(filePath) << " bytes\n";
            std::cout << "  Type: " << (FileUtils::isTextFile(filePath) ? "Text" : "Binary") << "\n";
            std::cout << "  Extension: " << FileUtils::getFileExtension(filePath) << "\n";
            break;
        }
        
        case 2: {
            std::string filePath = getInputPath("File path");
            if (filePath.empty()) return;
            
            if (!FileUtils::fileExists(filePath)) {
                std::cout << "Error: File does not exist.\n";
                return;
            }
            
            std::string backupPath = FileUtils::createBackup(filePath);
            if (!backupPath.empty()) {
                std::cout << "Backup created: " << backupPath << "\n";
            } else {
                std::cout << "Error: Failed to create backup.\n";
            }
            break;
        }
        
        case 3: {
            std::string filePath = getInputPath("File path to delete");
            if (filePath.empty()) return;
            
            if (!FileUtils::fileExists(filePath)) {
                std::cout << "Error: File does not exist.\n";
                return;
            }
            
            std::cout << "Warning: This will permanently delete the file!\n";
            std::cout << "Are you sure? (y/N): ";
            char confirm;
            std::cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                if (FileUtils::secureDelete(filePath, 3)) {
                    std::cout << "File securely deleted.\n";
                } else {
                    std::cout << "Error: Failed to delete file.\n";
                }
            } else {
                std::cout << "Deletion cancelled.\n";
            }
            break;
        }
        
        case 0:
            return;
            
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}

void handleMenuChoice(int choice) {
    // This function is kept for compatibility but not used in the simplified version
    (void)choice;
}

std::string getInputPath(const std::string& prompt) {
    std::string path;
    std::cout << prompt << ": ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, path);
    return path;
}

std::vector<std::string> getMultipleInputPaths() {
    std::vector<std::string> paths;
    std::cout << "Enter file paths (empty line to finish):\n";
    
    std::string path;
    while (true) {
        std::getline(std::cin, path);
        if (path.empty()) break;
        paths.push_back(path);
    }
    
    return paths;
}

std::vector<uint8_t> getHexSeed() {
    std::vector<uint8_t> seed;
    std::cout << "Enter hex seed (e.g., 0123456789ABCDEF): ";
    std::string hexString;
    std::cin >> hexString;
    
    // Convert hex string to bytes
    for (size_t i = 0; i < hexString.length(); i += 2) {
        if (i + 1 < hexString.length()) {
            std::string byteString = hexString.substr(i, 2);
            try {
                uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
                seed.push_back(byte);
            } catch (...) {
                std::cout << "Error: Invalid hex string.\n";
                return {};
            }
        }
    }
    
    return seed;
}