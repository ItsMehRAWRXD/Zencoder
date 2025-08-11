#include "file_utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <random>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

namespace FileUtils {

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool directoryExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool createDirectory(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (...) {
        return false;
    }
}

bool deleteFile(const std::string& path) {
    try {
        return std::filesystem::remove(path);
    } catch (...) {
        return false;
    }
}

bool copyFile(const std::string& source, const std::string& destination) {
    try {
        std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
        return true;
    } catch (...) {
        return false;
    }
}

bool moveFile(const std::string& source, const std::string& destination) {
    try {
        std::filesystem::rename(source, destination);
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<uint8_t> readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }
    
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return {};
    }
    
    return buffer;
}

bool writeFile(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return file.good();
}

bool writeFile(const std::string& path, const std::string& data) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    
    file << data;
    return file.good();
}

uint64_t getFileSize(const std::string& path) {
    try {
        return std::filesystem::file_size(path);
    } catch (...) {
        return 0;
    }
}

std::string getFileExtension(const std::string& path) {
    std::filesystem::path fsPath(path);
    return fsPath.extension().string();
}

std::string getFileName(const std::string& path) {
    std::filesystem::path fsPath(path);
    return fsPath.filename().string();
}

std::string getDirectoryPath(const std::string& path) {
    std::filesystem::path fsPath(path);
    return fsPath.parent_path().string();
}

std::string getAbsolutePath(const std::string& path) {
    try {
        return std::filesystem::absolute(path).string();
    } catch (...) {
        return path;
    }
}

std::vector<std::string> listFiles(const std::string& directory) {
    std::vector<std::string> files;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
    } catch (...) {
        // Return empty vector on error
    }
    return files;
}

std::vector<std::string> listDirectories(const std::string& directory) {
    std::vector<std::string> dirs;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_directory()) {
                dirs.push_back(entry.path().string());
            }
        }
    } catch (...) {
        // Return empty vector on error
    }
    return dirs;
}

std::vector<std::string> listAll(const std::string& directory) {
    std::vector<std::string> all;
    try {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            all.push_back(entry.path().string());
        }
    } catch (...) {
        // Return empty vector on error
    }
    return all;
}

bool removeDirectory(const std::string& path) {
    try {
        return std::filesystem::remove(path);
    } catch (...) {
        return false;
    }
}

bool removeDirectoryRecursive(const std::string& path) {
    try {
        return std::filesystem::remove_all(path) > 0;
    } catch (...) {
        return false;
    }
}

std::string joinPath(const std::string& path1, const std::string& path2) {
    std::filesystem::path p1(path1);
    std::filesystem::path p2(path2);
    return (p1 / p2).string();
}

std::string normalizePath(const std::string& path) {
    try {
        return std::filesystem::canonical(path).string();
    } catch (...) {
        return std::filesystem::absolute(path).string();
    }
}

bool isAbsolutePath(const std::string& path) {
    std::filesystem::path fsPath(path);
    return fsPath.is_absolute();
}

bool isRelativePath(const std::string& path) {
    return !isAbsolutePath(path);
}

bool isTextFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    char buffer[1024];
    file.read(buffer, sizeof(buffer));
    std::streamsize bytesRead = file.gcount();
    
    // Check for null bytes
    for (std::streamsize i = 0; i < bytesRead; ++i) {
        if (buffer[i] == 0) {
            return false;
        }
    }
    
    return true;
}

bool isBinaryFile(const std::string& path) {
    return !isTextFile(path);
}

bool isExecutableFile(const std::string& path) {
    std::string ext = getFileExtension(path);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return ext == ".exe" || ext == ".dll" || ext == ".so" || ext == ".dylib";
}

bool isImageFile(const std::string& path) {
    std::string ext = getFileExtension(path);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif" || 
           ext == ".bmp" || ext == ".tiff" || ext == ".webp";
}

bool isArchiveFile(const std::string& path) {
    std::string ext = getFileExtension(path);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return ext == ".zip" || ext == ".rar" || ext == ".7z" || ext == ".tar" || 
           ext == ".gz" || ext == ".bz2" || ext == ".xz";
}

bool secureDelete(const std::string& path, int passes) {
    if (!fileExists(path)) {
        return false;
    }
    
    uint64_t fileSize = getFileSize(path);
    if (fileSize == 0) {
        return deleteFile(path);
    }
    
    std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
    if (!file.is_open()) {
        return false;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    
    std::vector<uint8_t> buffer(4096);
    
    for (int pass = 0; pass < passes; ++pass) {
        file.seekg(0);
        uint64_t remaining = fileSize;
        
        while (remaining > 0) {
            size_t toWrite = std::min(remaining, static_cast<uint64_t>(buffer.size()));
            
            // Fill buffer with random data
            for (size_t i = 0; i < toWrite; ++i) {
                buffer[i] = dist(gen);
            }
            
            file.write(reinterpret_cast<const char*>(buffer.data()), toWrite);
            remaining -= toWrite;
        }
        
        file.flush();
    }
    
    file.close();
    return deleteFile(path);
}

bool setFilePermissions(const std::string& path, uint32_t permissions) {
#ifdef _WIN32
    // Windows implementation would go here
    return true;
#else
    return chmod(path.c_str(), permissions) == 0;
#endif
}

uint32_t getFilePermissions(const std::string& path) {
#ifdef _WIN32
    // Windows implementation would go here
    return 0;
#else
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return st.st_mode & 0777;
    }
    return 0;
#endif
}

std::string createBackup(const std::string& path) {
    if (!fileExists(path)) {
        return "";
    }
    
    std::string backupPath = path + ".backup";
    if (copyFile(path, backupPath)) {
        return backupPath;
    }
    return "";
}

bool restoreBackup(const std::string& backupPath, const std::string& targetPath) {
    if (!fileExists(backupPath)) {
        return false;
    }
    
    return copyFile(backupPath, targetPath);
}

bool isValidPath(const std::string& path) {
    try {
        std::filesystem::path fsPath(path);
        return true;
    } catch (...) {
        return false;
    }
}

bool isSafePath(const std::string& path) {
    std::string normalized = normalizePath(path);
    
    // Check for dangerous patterns
    if (normalized.find("..") != std::string::npos) {
        return false;
    }
    
    // Add more safety checks as needed
    return true;
}

std::string sanitizePath(const std::string& path) {
    std::string sanitized = path;
    
    // Remove null characters
    sanitized.erase(std::remove(sanitized.begin(), sanitized.end(), '\0'), sanitized.end());
    
    // Replace dangerous characters
    std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
    
    return sanitized;
}

} // namespace FileUtils