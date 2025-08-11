#ifndef FILE_ENC_DEC_H
#define FILE_ENC_DEC_H

#include <string>
#include <vector>
#include <cstdint>

class FileEncDec {
public:
    FileEncDec();
    ~FileEncDec();
    
    bool encryptFile(const std::string& inputPath, const std::string& outputPath, const std::vector<uint8_t>& key);
    bool decryptFile(const std::string& inputPath, const std::string& outputPath, const std::vector<uint8_t>& key);
};

#endif // FILE_ENC_DEC_H