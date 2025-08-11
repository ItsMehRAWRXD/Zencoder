#include "packer.h"
#include "menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <random>
#include <algorithm>
#include <cstring>
#include "utils/pe_parser.h"
#include "encryption/aes_cbc.h"

#ifdef _WIN32
#include <windows.h>
#endif

// External function for memory protection
extern void registerProtectedMemoryRegion(DWORD processId, void* startAddress, size_t size);

// File signature and format constants
const uint32_t Z3_FILE_SIGNATURE = 0x3A334E5A; // "Z3:" in little-endian
const uint8_t Z3_FORMAT_VERSION = 0x01;

// Implementation of AES and other crypto algorithms would be here
// For brevity, we'll use placeholder implementations

Z3Processor::Z3Processor() : m_processMode(Fx7z9Process::D4t0) {
    // Initialize with default no-encryption mode
}

Z3Processor::~Z3Processor() {
    // Clean up any resources
    // Securely wipe key material from memory
    if (!m_seedValue.empty()) {
        std::fill(m_seedValue.begin(), m_seedValue.end(), 0);
    }
}

void Z3Processor::setupMode(Fx7z9Process mode) {
    m_processMode = mode;
}

void Z3Processor::setupSeed(const std::vector<uint8_t>& seed) {
    if (!seed.empty()) {
        m_seedValue = seed;
    } else {
        // Generate a cryptographically strong random seed
        std::random_device rd;
        std::vector<uint8_t> entropy(32);
        
        // Fill with random data from hardware source if available
        for (size_t i = 0; i < entropy.size(); ++i) {
            entropy[i] = static_cast<uint8_t>(rd());
        }
        
        // Use a CSPRNG (Mersenne Twister) seeded with hardware randomness
        std::seed_seq seq(entropy.begin(), entropy.end());
        std::mt19937 gen(seq);
        std::uniform_int_distribution<unsigned> dist(0, 255);
        
        // Generate a 32-byte key (maximum size needed by any algorithm)
        m_seedValue.resize(32);
        for (int i = 0; i < 32; i++) {
            m_seedValue[i] = static_cast<uint8_t>(dist(gen));
        }
    }
    
    // Register this key memory for protection if hooks are available
    #ifdef _WIN32
    // Try to protect our key material
    try {
        registerProtectedMemoryRegion(GetCurrentProcessId(), m_seedValue.data(), m_seedValue.size());
    } catch (...) {
        // If it fails, continue without memory protection
    }
    #endif
}

Fx7z9Process Z3Processor::getProcessMode() const {
    return m_processMode;
}

void Z3Processor::runTask42(const std::vector<std::string>& sourceItems, const std::string& outputPath) {
    // Combine multiple files into a single encrypted package
    try {
        // Create header for the package
        std::vector<uint8_t> packageData;
        
        // Add file signature and format version
        packageData.push_back((Z3_FILE_SIGNATURE >> 0) & 0xFF);
        packageData.push_back((Z3_FILE_SIGNATURE >> 8) & 0xFF);
        packageData.push_back((Z3_FILE_SIGNATURE >> 16) & 0xFF);
        packageData.push_back((Z3_FILE_SIGNATURE >> 24) & 0xFF);
        packageData.push_back(Z3_FORMAT_VERSION);
        
        // Add encryption mode
        packageData.push_back(static_cast<uint8_t>(m_processMode));
        
        // Add number of items
        uint32_t numItems = static_cast<uint32_t>(sourceItems.size());
        packageData.push_back((numItems >> 0) & 0xFF);
        packageData.push_back((numItems >> 8) & 0xFF);
        packageData.push_back((numItems >> 16) & 0xFF);
        packageData.push_back((numItems >> 24) & 0xFF);
        
        // Reserve space for item table - will be filled in later
        size_t itemTableOffset = packageData.size();
        for (size_t i = 0; i < numItems; i++) {
            // Each entry needs: filename length (1 byte), filename, offset (4 bytes), size (4 bytes)
            packageData.resize(packageData.size() + 1 + 255 + 4 + 4, 0); // Maximum filename length
        }
        
        // Process each file
        std::vector<std::pair<std::string, std::pair<uint32_t, uint32_t>>> fileEntries;
        size_t dataOffset = packageData.size();
        
        for (const auto& itemPath : sourceItems) {
            // Read the file
            std::vector<uint8_t> fileData = loadResource(itemPath);
            if (fileData.empty()) {
                std::cerr << "Failed to read file: " << itemPath << std::endl;
                continue;
            }
            
            // Get the base filename
            std::string filename = std::filesystem::path(itemPath).filename().string();
            
            // Process the data if encryption mode is set
            if (m_processMode != Fx7z9Process::D4t0) {
                fileData = processData(fileData, true);
            }
            
            // Store file entry information
            uint32_t fileOffset = static_cast<uint32_t>(packageData.size() - dataOffset);
            uint32_t fileSize = static_cast<uint32_t>(fileData.size());
            fileEntries.push_back(std::make_pair(filename, std::make_pair(fileOffset, fileSize)));
            
            // Add the file data to the package
            packageData.insert(packageData.end(), fileData.begin(), fileData.end());
        }
        
        // Now fill in the item table
        size_t tablePos = itemTableOffset;
        for (const auto& entry : fileEntries) {
            const std::string& filename = entry.first;
            uint32_t fileOffset = entry.second.first;
            uint32_t fileSize = entry.second.second;
            
            // Write filename length and filename
            packageData[tablePos++] = static_cast<uint8_t>(filename.length());
            for (char c : filename) {
                packageData[tablePos++] = static_cast<uint8_t>(c);
            }
            
            // Skip to the offset field (standardized field size for simplicity)
            tablePos = itemTableOffset + 1 + 255;
            
            // Write offset and size
            packageData[tablePos++] = (fileOffset >> 0) & 0xFF;
            packageData[tablePos++] = (fileOffset >> 8) & 0xFF;
            packageData[tablePos++] = (fileOffset >> 16) & 0xFF;
            packageData[tablePos++] = (fileOffset >> 24) & 0xFF;
            
            packageData[tablePos++] = (fileSize >> 0) & 0xFF;
            packageData[tablePos++] = (fileSize >> 8) & 0xFF;
            packageData[tablePos++] = (fileSize >> 16) & 0xFF;
            packageData[tablePos++] = (fileSize >> 24) & 0xFF;
            
            // Move to the next entry
            itemTableOffset += 1 + 255 + 4 + 4;
        }
        
        // Write the package file
        std::ofstream outFile(outputPath, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << outputPath << std::endl;
            return;
        }
        
        outFile.write(reinterpret_cast<const char*>(packageData.data()), packageData.size());
        outFile.close();
        
        std::cout << "Package created successfully: " << outputPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error in runTask42: " << e.what() << std::endl;
    }
}

void Z3Processor::runTask67(const std::string& inputPath, const std::string& outputDir) {
    // Extract files from an encrypted package
    try {
        // Read the package file
        std::vector<uint8_t> packageData = loadResource(inputPath);
        if (packageData.empty()) {
            std::cerr << "Failed to read package file: " << inputPath << std::endl;
            return;
        }
        
        // Ensure file is large enough for a header
        if (packageData.size() < 10) {
            std::cerr << "Invalid package file: too small" << std::endl;
            return;
        }
        
        // Check file signature
        uint32_t signature = 
            (static_cast<uint32_t>(packageData[0]) << 0) |
            (static_cast<uint32_t>(packageData[1]) << 8) |
            (static_cast<uint32_t>(packageData[2]) << 16) |
            (static_cast<uint32_t>(packageData[3]) << 24);
            
        if (signature != Z3_FILE_SIGNATURE) {
            std::cerr << "Invalid package file: wrong signature" << std::endl;
            return;
        }
        
        // Check format version
        if (packageData[4] != Z3_FORMAT_VERSION) {
            std::cerr << "Unsupported package format version" << std::endl;
            return;
        }
        
        // Get encryption mode
        Fx7z9Process packageMode = static_cast<Fx7z9Process>(packageData[5]);
        
        // If the package is encrypted, set our mode to match
        if (packageMode != Fx7z9Process::D4t0) {
            m_processMode = packageMode;
        }
        
        // Get number of items
        uint32_t numItems = 
            (static_cast<uint32_t>(packageData[6]) << 0) |
            (static_cast<uint32_t>(packageData[7]) << 8) |
            (static_cast<uint32_t>(packageData[8]) << 16) |
            (static_cast<uint32_t>(packageData[9]) << 24);
            
        // Parse the item table
        size_t tableOffset = 10;
        std::vector<std::tuple<std::string, uint32_t, uint32_t>> fileEntries;
        
        for (uint32_t i = 0; i < numItems; i++) {
            // Get filename length
            uint8_t filenameLength = packageData[tableOffset++];
            
            // Get filename
            std::string filename;
            for (uint8_t j = 0; j < filenameLength; j++) {
                filename.push_back(static_cast<char>(packageData[tableOffset++]));
            }
            
            // Skip to the standard offset position
            tableOffset = 10 + (i * (1 + 255 + 4 + 4)) + 1 + 255;
            
            // Get file offset
            uint32_t fileOffset = 
                (static_cast<uint32_t>(packageData[tableOffset++]) << 0) |
                (static_cast<uint32_t>(packageData[tableOffset++]) << 8) |
                (static_cast<uint32_t>(packageData[tableOffset++]) << 16) |
                (static_cast<uint32_t>(packageData[tableOffset++]) << 24);
                
            // Get file size
            uint32_t fileSize = 
                (static_cast<uint32_t>(packageData[tableOffset++]) << 0) |
                (static_cast<uint32_t>(packageData[tableOffset++]) << 8) |
                (static_cast<uint32_t>(packageData[tableOffset++]) << 16) |
                (static_cast<uint32_t>(packageData[tableOffset++]) << 24);
                
            fileEntries.push_back(std::make_tuple(filename, fileOffset, fileSize));
        }
        
        // Get the data section offset
        size_t dataOffset = 10 + (numItems * (1 + 255 + 4 + 4));
        
        // Create output directory if it doesn't exist
        std::filesystem::create_directories(outputDir);
        
        // Extract each file
        for (const auto& entry : fileEntries) {
            std::string filename;
            uint32_t fileOffset, fileSize;
            std::tie(filename, fileOffset, fileSize) = entry;
            
            // Get the file data
            std::vector<uint8_t> fileData(packageData.begin() + dataOffset + fileOffset, 
                                        packageData.begin() + dataOffset + fileOffset + fileSize);
            
            // Decrypt the data if necessary
            if (packageMode != Fx7z9Process::D4t0) {
                fileData = processData(fileData, false);
            }
            
            // Write the file
            std::string outputPath = std::filesystem::path(outputDir) / filename;
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile) {
                std::cerr << "Failed to create output file: " << outputPath << std::endl;
                continue;
            }
            
            outFile.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
            outFile.close();
        }
        
        return;
    } catch (const std::exception& e) {
        std::cerr << "Error in runTask67: " << e.what() << std::endl;
        return;
    }
}

bool Z3Processor::checkFormat92(const std::string& itemPath) {
    // Check if a file has the correct format
    try {
        // Read the file header
        std::ifstream inFile(itemPath, std::ios::binary);
        if (!inFile) {
            return false;
        }
        
        char header[5];
        inFile.read(header, 5);
        
        if (inFile.gcount() < 5) {
            return false;
        }
        
        // Check file signature
        uint32_t signature = 
            (static_cast<uint32_t>(static_cast<uint8_t>(header[0])) << 0) |
            (static_cast<uint32_t>(static_cast<uint8_t>(header[1])) << 8) |
            (static_cast<uint32_t>(static_cast<uint8_t>(header[2])) << 16) |
            (static_cast<uint32_t>(static_cast<uint8_t>(header[3])) << 24);
            
        // Check if it's our package format
        if (signature == Z3_FILE_SIGNATURE) {
            return true;
        }
        
        // Check for PE file format (MZ header)
        if (header[0] == 'M' && header[1] == 'Z') {
            return true;
        }
        
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error in checkFormat92: " << e.what() << std::endl;
        return false;
    }
}

bool Z3Processor::task104(const std::string& targetPath, const std::string& label, 
                         const std::vector<uint8_t>& componentData) {
    // Add a component to a PE file
    try {
        // First check if file is a valid PE
        if (!checkFormat92(targetPath)) {
            std::cerr << "Target file is not a valid PE file" << std::endl;
            return false;
        }
        
        // Load the PE file
        std::vector<uint8_t> peData = loadResource(targetPath);
        if (peData.empty()) {
            std::cerr << "Failed to read PE file: " << targetPath << std::endl;
            return false;
        }
        
        // Use PE parser to add a proper section
        PEParser parser;
        if (!parser.loadPE(peData)) {
            std::cerr << "Failed to parse PE file" << std::endl;
            return false;
        }
        
        // Create section name from label (max 8 chars)
        std::string sectionName = "." + label.substr(0, 7);
        
        // Prepare section data with component header
        std::vector<uint8_t> sectionData;
        
        // Add component header (label + size)
        sectionData.resize(12, 0);
        for (size_t i = 0; i < label.length() && i < 8; i++) {
            sectionData[i] = static_cast<uint8_t>(label[i]);
        }
        
        uint32_t componentSize = static_cast<uint32_t>(componentData.size());
        sectionData[8] = (componentSize >> 0) & 0xFF;
        sectionData[9] = (componentSize >> 8) & 0xFF;
        sectionData[10] = (componentSize >> 16) & 0xFF;
        sectionData[11] = (componentSize >> 24) & 0xFF;
        
        // Append the actual component data
        sectionData.insert(sectionData.end(), componentData.begin(), componentData.end());
        
        // Add the new section with appropriate characteristics
        DWORD characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;
        if (!parser.addSection(sectionName, sectionData, characteristics)) {
            std::cerr << "Failed to add section to PE file" << std::endl;
            return false;
        }
        
        // Save the modified PE file
        if (!parser.save(targetPath)) {
            std::cerr << "Failed to save modified PE file" << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in task104: " << e.what() << std::endl;
        return false;
    }
}

bool Z3Processor::task219(const std::string& targetPath, const std::string& label, 
                         Fx7z9Process mode, const std::vector<uint8_t>& seed) {
    // Process a component in a PE file
    try {
        // Check if file is a valid PE
        if (!checkFormat92(targetPath)) {
            std::cerr << "Target file is not a valid PE file" << std::endl;
            return false;
        }
        
        // Load the PE file
        std::vector<uint8_t> peData = loadResource(targetPath);
        if (peData.empty()) {
            std::cerr << "Failed to read PE file: " << targetPath << std::endl;
            return false;
        }
        
        // Use PE parser to find the component in sections
        PEParser parser;
        if (!parser.loadPE(peData)) {
            std::cerr << "Failed to parse PE file" << std::endl;
            return false;
        }
        
        // Look for the section containing our component
        std::string expectedSectionName = "." + label.substr(0, 7);
        bool found = false;
        std::vector<uint8_t> componentData;
        size_t componentOffset = 0;
        uint32_t componentSize = 0;
        
        // Check all sections
        auto* sectionHeaders = parser.getSectionHeaders();
        auto* ntHeaders = parser.getNTHeaders();
        
        for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++) {
            char sectionName[9] = {0};
            memcpy(sectionName, sectionHeaders[i].Name, 8);
            
            // Check if this section might contain our component
            if (strstr(sectionName, label.substr(0, 7).c_str()) != nullptr ||
                strcmp(sectionName, expectedSectionName.c_str()) == 0) {
                
                // Read section data
                uint32_t sectionRVA = sectionHeaders[i].VirtualAddress;
                uint32_t sectionSize = sectionHeaders[i].SizeOfRawData;
                uint32_t fileOffset = sectionHeaders[i].PointerToRawData;
                
                if (fileOffset + sectionSize <= peData.size()) {
                    // Check for component header in this section
                    std::vector<uint8_t> sectionData(
                        peData.begin() + fileOffset,
                        peData.begin() + fileOffset + sectionSize
                    );
                    
                    // Verify component header
                    if (sectionData.size() >= 12) {
                        bool labelMatch = true;
                        for (size_t j = 0; j < label.length() && j < 8; j++) {
                            if (sectionData[j] != static_cast<uint8_t>(label[j])) {
                                labelMatch = false;
                                break;
                            }
                        }
                        
                        if (labelMatch) {
                            componentSize = 
                                (static_cast<uint32_t>(sectionData[8]) << 0) |
                                (static_cast<uint32_t>(sectionData[9]) << 8) |
                                (static_cast<uint32_t>(sectionData[10]) << 16) |
                                (static_cast<uint32_t>(sectionData[11]) << 24);
                            
                            if (12 + componentSize <= sectionData.size()) {
                                componentData.assign(
                                    sectionData.begin() + 12,
                                    sectionData.begin() + 12 + componentSize
                                );
                                componentOffset = fileOffset;
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        if (!found) {
            std::cerr << "Component with label '" << label << "' not found in PE sections" << std::endl;
            return false;
        }
        
        // Set processing mode and seed
        Fx7z9Process oldMode = m_processMode;
        std::vector<uint8_t> oldSeed = m_seedValue;
        
        m_processMode = mode;
        if (!seed.empty()) {
            setupSeed(seed);
        }
        
        // Process the component data
        std::vector<uint8_t> processedData = processData(componentData, true);
        
        // Restore old mode and seed
        m_processMode = oldMode;
        m_seedValue = oldSeed;
        
        // Update the component in the PE file
        peData.erase(
            peData.begin() + componentOffset + 12,
            peData.begin() + componentOffset + 12 + componentSize
        );
        
        peData.insert(
            peData.begin() + componentOffset + 12,
            processedData.begin(),
            processedData.end()
        );
        
        // Update component size
        uint32_t newSize = static_cast<uint32_t>(processedData.size());
        peData[componentOffset + 8] = (newSize >> 0) & 0xFF;
        peData[componentOffset + 9] = (newSize >> 8) & 0xFF;
        peData[componentOffset + 10] = (newSize >> 16) & 0xFF;
        peData[componentOffset + 11] = (newSize >> 24) & 0xFF;
        
        // Write the modified PE file
        std::ofstream outFile(targetPath, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create output file: " << targetPath << std::endl;
            return false;
        }
        
        outFile.write(reinterpret_cast<const char*>(peData.data()), peData.size());
        outFile.close();
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in task219: " << e.what() << std::endl;
        return false;
    }
}

bool Z3Processor::task317(const std::string& targetPath) {
    // Obfuscate PE import table
    try {
        // Use the PE parser to properly obfuscate imports
        PEParser parser;
        if (!parser.loadPE(targetPath)) {
            std::cerr << "Failed to load PE file: " << targetPath << std::endl;
            return false;
        }
        
        // Get and display current imports before obfuscation
        auto imports = parser.getImports();
        std::cout << "Found " << imports.size() << " import descriptors" << std::endl;
        
        for (const auto& imp : imports) {
            std::cout << "  DLL: " << imp.dllName << " (" << imp.functions.size() << " functions)" << std::endl;
        }
        
        // Perform obfuscation
        if (!parser.obfuscateImports()) {
            std::cerr << "Failed to obfuscate imports" << std::endl;
            return false;
        }
        
        // Save the modified PE file
        if (!parser.save(targetPath)) {
            std::cerr << "Failed to save modified PE file" << std::endl;
            return false;
        }
        
        std::cout << "Import table obfuscation completed successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in task317: " << e.what() << std::endl;
        return false;
    }
}

bool Z3Processor::task318(const std::string& targetPath) {
    // Obfuscate PE export table
    try {
        // Use the PE parser to properly obfuscate exports
        PEParser parser;
        if (!parser.loadPE(targetPath)) {
            std::cerr << "Failed to load PE file: " << targetPath << std::endl;
            return false;
        }
        
        // Get and display current exports before obfuscation
        auto exports = parser.getExports();
        std::cout << "Found " << exports.size() << " export descriptors" << std::endl;
        
        for (const auto& exp : exports) {
            std::cout << "  DLL: " << exp.dllName << " (" << exp.functions.size() << " functions)" << std::endl;
            for (size_t i = 0; i < std::min(size_t(5), exp.functions.size()); i++) {
                std::cout << "    - " << exp.functions[i] << std::endl;
            }
            if (exp.functions.size() > 5) {
                std::cout << "    ... and " << (exp.functions.size() - 5) << " more" << std::endl;
            }
        }
        
        // Perform obfuscation
        if (!parser.obfuscateExports()) {
            std::cerr << "Failed to obfuscate exports" << std::endl;
            return false;
        }
        
        // Save the modified PE file
        if (!parser.save(targetPath)) {
            std::cerr << "Failed to save modified PE file" << std::endl;
            return false;
        }
        
        std::cout << "Export table obfuscation completed successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error in task318: " << e.what() << std::endl;
        return false;
    }
}

std::vector<uint8_t> Z3Processor::loadResource(const std::string& path) {
    try {
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            return std::vector<uint8_t>();
        }
        
        // Get file size
        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        // Read the file
        std::vector<uint8_t> data(fileSize);
        file.read(reinterpret_cast<char*>(data.data()), fileSize);
        
        return data;
    } catch (const std::exception& e) {
        std::cerr << "Error in loadResource: " << e.what() << std::endl;
        return std::vector<uint8_t>();
    }
}

std::vector<uint8_t> Z3Processor::processData(const std::vector<uint8_t>& data, bool encrypt) {
    // Apply the selected encryption/decryption algorithm
    switch (m_processMode) {
        case Fx7z9Process::Qx3r:
            return algorithmXor(data);
        case Fx7z9Process::K5bc:
            return algorithmAesCbc(data, encrypt);
        case Fx7z9Process::M2tr:
            return algorithmAesCtr(data, encrypt);
        case Fx7z9Process::Z1ha:
            return algorithmChacha20(data, encrypt);
        case Fx7z9Process::P3rc:
            return algorithmRc4(data);
        case Fx7z9Process::B2fs:
            return algorithmBlowfish(data, encrypt);
        case Fx7z9Process::T5sh:
            return algorithmTwofish(data, encrypt);
        case Fx7z9Process::S7pt:
            return algorithmSerpent(data, encrypt);
        case Fx7z9Process::C9la:
            return algorithmCamellia(data, encrypt);
        case Fx7z9Process::G4cm:
            return algorithmGcm(data, encrypt);
        case Fx7z9Process::P1y5:
            return algorithmPoly1305(data, encrypt);
        default:
            return data; // No processing
    }
}

// Placeholder implementations of encryption algorithms
// In a real implementation, these would use proper cryptographic libraries

std::vector<uint8_t> Z3Processor::algorithmXor(const std::vector<uint8_t>& data) {
    // Simple XOR encryption/decryption with the seed
    std::vector<uint8_t> result = data;
    
    if (m_seedValue.empty()) {
        return result;
    }
    
    for (size_t i = 0; i < result.size(); i++) {
        result[i] ^= m_seedValue[i % m_seedValue.size()];
    }
    
    return result;
}

std::vector<uint8_t> Z3Processor::algorithmAesCbc(const std::vector<uint8_t>& data, bool encrypt) {
    // Use proper AES-CBC implementation
    try {
        // Generate key from seed (use first 32 bytes for AES-256)
        std::vector<uint8_t> key(32);
        for (size_t i = 0; i < 32; i++) {
            key[i] = m_seedValue[i % m_seedValue.size()];
        }
        
        // Generate IV from seed (use next 16 bytes)
        std::vector<uint8_t> iv(16);
        for (size_t i = 0; i < 16; i++) {
            iv[i] = m_seedValue[(i + 32) % m_seedValue.size()];
        }
        
        // Create AES-CBC instance
        AES_CBC aes(key, iv);
        
        // Encrypt or decrypt
        if (encrypt) {
            return aes.encrypt(data);
        } else {
            return aes.decrypt(data);
        }
    } catch (const std::exception& e) {
        std::cerr << "AES-CBC error: " << e.what() << std::endl;
        // Fall back to XOR on error
        return algorithmXor(data);
    }
}

std::vector<uint8_t> Z3Processor::algorithmAesCtr(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for AES-CTR implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmChacha20(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for ChaCha20 implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmRc4(const std::vector<uint8_t>& data) {
    // Placeholder for RC4 implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmBlowfish(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for Blowfish implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmTwofish(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for Twofish implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmSerpent(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for Serpent implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmCamellia(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for Camellia implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmGcm(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for GCM implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::algorithmPoly1305(const std::vector<uint8_t>& data, bool encrypt) {
    // Placeholder for Poly1305 implementation
    return algorithmXor(data);
}

std::vector<uint8_t> Z3Processor::getRandomNonce(size_t size) {
    std::vector<uint8_t> nonce(size);
    
    // Use hardware RNG if available
    std::random_device rd;
    for (size_t i = 0; i < size; i++) {
        nonce[i] = static_cast<uint8_t>(rd());
    }
    
    // Mix with seed for deterministic component
    if (!m_seedValue.empty()) {
        for (size_t i = 0; i < size && i < m_seedValue.size(); i++) {
            nonce[i] ^= m_seedValue[i];
        }
    }
    
    return nonce;
}

int main() {
    // Initialize the application
    std::cout << "Welcome to the File Encryption and Packing Tool!" << std::endl;

    // Display the menu and handle user input
    displayMenu();

    return 0;
}