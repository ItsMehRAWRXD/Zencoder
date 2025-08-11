#ifndef AES_CBC_H
#define AES_CBC_H

#include <vector>
#include <cstdint>
#include <string>

class AES_CBC {
public:
    // Key sizes in bits
    enum KeySize {
        AES128 = 128,
        AES192 = 192,
        AES256 = 256
    };

    AES_CBC(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv);
    ~AES_CBC();

    // Encrypt data using AES-CBC
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    
    // Decrypt data using AES-CBC
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

    // Set new IV
    void setIV(const std::vector<uint8_t>& iv);

private:
    static const int BLOCK_SIZE = 16;  // AES block size is always 128 bits
    
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;
    int Nr;  // Number of rounds
    std::vector<uint32_t> expandedKey;
    
    // AES operations
    void expandKey();
    void encryptBlock(uint8_t* block);
    void decryptBlock(uint8_t* block);
    
    // AES round functions
    void subBytes(uint8_t* state);
    void invSubBytes(uint8_t* state);
    void shiftRows(uint8_t* state);
    void invShiftRows(uint8_t* state);
    void mixColumns(uint8_t* state);
    void invMixColumns(uint8_t* state);
    void addRoundKey(uint8_t* state, int round);
    
    // Utility functions
    uint8_t multiplyGF(uint8_t a, uint8_t b);
    void xorBlocks(uint8_t* a, const uint8_t* b);
    std::vector<uint8_t> addPKCS7Padding(const std::vector<uint8_t>& data);
    std::vector<uint8_t> removePKCS7Padding(const std::vector<uint8_t>& data);
};

#endif // AES_CBC_H