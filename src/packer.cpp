#include "packer.h"
#include "../src/utils/file_utils.h"
#include <iostream>

FileProcessor::FileProcessor() {
    // Constructor - nothing to initialize
}

FileProcessor::~FileProcessor() {
    // Destructor - nothing to clean up
}

bool FileProcessor::encryptFile(const std::string& inputPath, const std::string& outputPath) {
    // Read input file
    auto data = readFile(inputPath);
    if (data.empty()) {
        std::cout << "Error: Could not read input file: " << inputPath << std::endl;
        return false;
    }
    
    // For now, just copy the file (placeholder for encryption)
    return writeFile(outputPath, data);
}

bool FileProcessor::decryptFile(const std::string& inputPath, const std::string& outputPath) {
    // Read input file
    auto data = readFile(inputPath);
    if (data.empty()) {
        std::cout << "Error: Could not read input file: " << inputPath << std::endl;
        return false;
    }
    
    // For now, just copy the file (placeholder for decryption)
    return writeFile(outputPath, data);
}

bool FileProcessor::isValidFile(const std::string& path) {
    return FileUtils::fileExists(path);
}

std::vector<uint8_t> FileProcessor::readFile(const std::string& path) {
    return FileUtils::readFile(path);
}

bool FileProcessor::writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    return FileUtils::writeFile(path, data);
}