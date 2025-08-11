#ifndef ENCRYPTION_BASE_H
#define ENCRYPTION_BASE_H

#include <vector>
#include <cstdint>

class EncryptionBase {
public:
    EncryptionBase();
    virtual ~EncryptionBase();
    
    // Pure virtual functions that derived classes must implement
    virtual std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext) = 0;
    virtual std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext) = 0;
};

#endif // ENCRYPTION_BASE_H