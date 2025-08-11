#include "menu.h"
#include "../src/encryption/aes_cbc.h"
#include "../src/utils/file_utils.h"
#include "../src/stub_generator.h"
#include "../src/drag_drop.h"
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
        std::cout << "3. Stub Generator\n";
        std::cout << "4. Drag and Drop Interface\n";
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
        } else if (choice == 3) {
            displayStubMenu();
            int stubChoice;
            std::cin >> stubChoice;
            handleStubChoice(stubChoice);
        } else if (choice == 4) {
            displayDragDropMenu();
            int dragChoice;
            std::cin >> dragChoice;
            handleDragDropChoice(dragChoice);
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

std::vector<std::string> getMultipleInputPaths(const std::string& prompt) {
    std::vector<std::string> paths;
    std::cout << prompt << " (empty line to finish):\n";
    
    std::string path;
    while (true) {
        std::getline(std::cin, path);
        if (path.empty()) break;
        paths.push_back(path);
    }
    
    return paths;
}

std::string getHexSeed(const std::string& prompt) {
    std::cout << prompt << " (e.g., 0123456789ABCDEF): ";
    std::string hexString;
    std::cin >> hexString;
    return hexString;
}

// Stub Generator Menu Functions
void displayStubMenu() {
    std::cout << "\n==== Stub Generator ====\n";
    std::cout << "1. Generate Basic Stub\n";
    std::cout << "2. Generate Self-Extracting Stub\n";
    std::cout << "3. Attach File to Stub\n";
    std::cout << "4. Generate Anti-Analysis Stub\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handleStubChoice(int choice) {
    StubGenerator generator;
    
    switch (choice) {
        case 1: {
            std::string outputPath = getInputPath("Output stub path");
            if (outputPath.empty()) return;
            
            StubConfig config;
            config.encryptionEnabled = true;
            config.antiDebug = false;
            config.antiVM = false;
            config.persistence = false;
            
            if (generator.generateStub(outputPath, config)) {
                std::cout << "Basic stub generated: " << outputPath << "\n";
            } else {
                std::cout << "Error: Failed to generate stub.\n";
            }
            break;
        }
        
        case 2: {
            std::string outputPath = getInputPath("Output stub path");
            if (outputPath.empty()) return;
            
            std::string filePath = getInputPath("File to embed");
            if (filePath.empty()) return;
            
            if (!FileUtils::fileExists(filePath)) {
                std::cout << "Error: File does not exist.\n";
                return;
            }
            
            StubConfig config;
            config.encryptionEnabled = true;
            config.antiDebug = true;
            config.antiVM = true;
            config.persistence = false;
            
            if (generator.generateSelfExtractingStub(outputPath, filePath, config)) {
                std::cout << "Self-extracting stub generated: " << outputPath << "\n";
            } else {
                std::cout << "Error: Failed to generate self-extracting stub.\n";
            }
            break;
        }
        
        case 3: {
            std::string stubPath = getInputPath("Stub file path");
            if (stubPath.empty()) return;
            
            std::string filePath = getInputPath("File to attach");
            if (filePath.empty()) return;
            
            std::string outputPath = getInputPath("Output executable path");
            if (outputPath.empty()) return;
            
            if (!FileUtils::fileExists(stubPath)) {
                std::cout << "Error: Stub file does not exist.\n";
                return;
            }
            
            if (!FileUtils::fileExists(filePath)) {
                std::cout << "Error: File to attach does not exist.\n";
                return;
            }
            
            if (generator.attachFileToStub(stubPath, filePath, outputPath)) {
                std::cout << "File attached to stub: " << outputPath << "\n";
            } else {
                std::cout << "Error: Failed to attach file to stub.\n";
            }
            break;
        }
        
        case 4: {
            std::string outputPath = getInputPath("Output stub path");
            if (outputPath.empty()) return;
            
            StubConfig config;
            config.encryptionEnabled = true;
            config.antiDebug = true;
            config.antiVM = true;
            config.persistence = true;
            
            if (generator.generateStub(outputPath, config)) {
                std::cout << "Anti-analysis stub generated: " << outputPath << "\n";
            } else {
                std::cout << "Error: Failed to generate anti-analysis stub.\n";
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

// Drag and Drop Menu Functions
void displayDragDropMenu() {
    std::cout << "\n==== Drag and Drop Interface ====\n";
    std::cout << "1. Initialize Drag and Drop\n";
    std::cout << "2. Enable/Disable Drag and Drop\n";
    std::cout << "3. Set Drop Callback\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handleDragDropChoice(int choice) {
    static DragDropHandler* handler = nullptr;
    
    switch (choice) {
        case 1: {
            if (handler) {
                delete handler;
            }
            
            handler = new DragDropHandler();
            
            // For console applications, we'll use a dummy window handle
            // In a real GUI application, you would pass the actual window handle
            HWND consoleWindow = GetConsoleWindow();
            
            if (handler->initialize(consoleWindow)) {
                std::cout << "Drag and drop initialized successfully.\n";
            } else {
                std::cout << "Error: Failed to initialize drag and drop.\n";
                delete handler;
                handler = nullptr;
            }
            break;
        }
        
        case 2: {
            if (!handler) {
                std::cout << "Error: Drag and drop not initialized.\n";
                return;
            }
            
            std::cout << "Enable drag and drop? (y/N): ";
            char enable;
            std::cin >> enable;
            
            if (enable == 'y' || enable == 'Y') {
                if (handler->enableDragDrop(true)) {
                    std::cout << "Drag and drop enabled.\n";
                } else {
                    std::cout << "Error: Failed to enable drag and drop.\n";
                }
            } else {
                if (handler->enableDragDrop(false)) {
                    std::cout << "Drag and drop disabled.\n";
                } else {
                    std::cout << "Error: Failed to disable drag and drop.\n";
                }
            }
            break;
        }
        
        case 3: {
            if (!handler) {
                std::cout << "Error: Drag and drop not initialized.\n";
                return;
            }
            
            std::cout << "Setting drop callback...\n";
            handler->setDropCallback([](const DragDrop::DropEvent& event) {
                std::cout << "Files dropped:\n";
                for (const auto& file : event.files) {
                    std::wcout << L"  " << file << L"\n";
                }
            });
            
            std::cout << "Drop callback set successfully.\n";
            break;
        }
        
        case 0:
            if (handler) {
                delete handler;
                handler = nullptr;
            }
            return;
            
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}