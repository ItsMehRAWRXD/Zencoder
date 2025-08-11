#ifndef CHACHA20_H
#define CHACHA20_H

#include <vector>
#include <cstdint>

class ChaCha20 {
public:
    ChaCha20(const std::vector<uint8_t>& key, const std::vector<uint8_t>& nonce);
    ~ChaCha20();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> nonce;
};

#endif // CHACHA20_H
