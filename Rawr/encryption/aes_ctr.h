#ifndef AES_CTR_H
#define AES_CTR_H

#include <vector>
#include <cstdint>

class AES_CTR {
public:
    AES_CTR(const std::vector<uint8_t>& key, const std::vector<uint8_t>& nonce);
    ~AES_CTR();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> nonce;
    std::vector<uint8_t> counter;
};

#endif // AES_CTR_H