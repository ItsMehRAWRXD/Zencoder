#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <vector>
#include <cstdint>

class Blowfish {
public:
    Blowfish(const std::vector<uint8_t>& key);
    ~Blowfish();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
};

#endif // BLOWFISH_H
