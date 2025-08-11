#ifndef SERPENT_H
#define SERPENT_H

#include <vector>
#include <cstdint>

class Serpent {
public:
    Serpent(const std::vector<uint8_t>& key);
    ~Serpent();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
};

#endif // SERPENT_H
