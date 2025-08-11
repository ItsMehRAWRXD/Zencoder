#pragma once

#include <vector>
#include <cstdint>
#include <string>

class AESCBC {
public:
    AESCBC();
    ~AESCBC();
    
    // Initialize with key and IV
    bool initialize(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv);
    
    // Encrypt data
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    
    // Decrypt data
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
    
    // Generate random key
    static std::vector<uint8_t> generateKey(size_t keySize = 32);
    
    // Generate random IV
    static std::vector<uint8_t> generateIV();
    
    // Get key size in bytes
    size_t getKeySize() const;
    
    // Get block size in bytes
    size_t getBlockSize() const;
    
    // Check if initialized
    bool isInitialized() const;
    
    // Clear sensitive data
    void clear();

private:
    class Impl;
    Impl* pImpl;
    
    // Disable copy constructor and assignment
    AESCBC(const AESCBC&) = delete;
    AESCBC& operator=(const AESCBC&) = delete;
};