#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>

namespace FileUtils {
    
    // File operations
    bool fileExists(const std::string& path);
    bool directoryExists(const std::string& path);
    bool createDirectory(const std::string& path);
    bool deleteFile(const std::string& path);
    bool copyFile(const std::string& source, const std::string& destination);
    bool moveFile(const std::string& source, const std::string& destination);
    
    // File reading and writing
    std::vector<uint8_t> readFile(const std::string& path);
    bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
    bool writeFile(const std::string& path, const std::string& data);
    
    // File information
    uint64_t getFileSize(const std::string& path);
    std::string getFileExtension(const std::string& path);
    std::string getFileName(const std::string& path);
    std::string getDirectoryPath(const std::string& path);
    std::string getAbsolutePath(const std::string& path);
    
    // Directory operations
    std::vector<std::string> listFiles(const std::string& directory);
    std::vector<std::string> listDirectories(const std::string& directory);
    std::vector<std::string> listAll(const std::string& directory);
    bool removeDirectory(const std::string& path);
    bool removeDirectoryRecursive(const std::string& path);
    
    // Path utilities
    std::string joinPath(const std::string& path1, const std::string& path2);
    std::string normalizePath(const std::string& path);
    bool isAbsolutePath(const std::string& path);
    bool isRelativePath(const std::string& path);
    
    // File type detection
    bool isTextFile(const std::string& path);
    bool isBinaryFile(const std::string& path);
    bool isExecutableFile(const std::string& path);
    bool isImageFile(const std::string& path);
    bool isArchiveFile(const std::string& path);
    
    // Security utilities
    bool secureDelete(const std::string& path, int passes = 3);
    bool setFilePermissions(const std::string& path, uint32_t permissions);
    uint32_t getFilePermissions(const std::string& path);
    
    // Backup utilities
    std::string createBackup(const std::string& path);
    bool restoreBackup(const std::string& backupPath, const std::string& targetPath);
    
    // Validation
    bool isValidPath(const std::string& path);
    bool isSafePath(const std::string& path);
    std::string sanitizePath(const std::string& path);
    
} // namespace FileUtils