#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <cstdint>

namespace FileUtils {

bool readFile(const std::string& path, std::vector<uint8_t>& data);
bool writeFile(const std::string& path, const std::vector<uint8_t>& data);
bool fileExists(const std::string& path);
size_t getFileSize(const std::string& path);

} // namespace FileUtils

#endif // FILE_UTILS_H