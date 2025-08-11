#include "file_helper.h"
#include <fstream>
#include <filesystem>

namespace FileUtils {

bool readFile(const std::string& path, std::vector<uint8_t>& data) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }
    
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    data.resize(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return file.good();
}

bool writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}

size_t getFileSize(const std::string& path) {
    if (!fileExists(path)) {
        return 0;
    }
    return std::filesystem::file_size(path);
}

} // namespace FileUtils