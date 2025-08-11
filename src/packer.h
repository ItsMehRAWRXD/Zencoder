#pragma once

#include <string>
#include <vector>
#include <cstdint>

// Simple file processor class
class FileProcessor {
public:
    FileProcessor();
    ~FileProcessor();
    
    // Basic file operations
    bool encryptFile(const std::string& inputPath, const std::string& outputPath);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath);
    
    // Utility methods
    bool isValidFile(const std::string& path);
    std::vector<uint8_t> readFile(const std::string& path);
    bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
    
private:
    // Implementation details
};