#ifndef CAMELLIA_H
#define CAMELLIA_H

#include <vector>
#include <cstdint>

class Camellia {
public:
    Camellia(const std::vector<uint8_t>& key);
    ~Camellia();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
};

#endif // CAMELLIA_H
