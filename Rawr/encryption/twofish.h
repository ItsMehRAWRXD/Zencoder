#ifndef TWOFISH_H
#define TWOFISH_H

#include <vector>
#include <cstdint>

class Twofish {
public:
    Twofish(const std::vector<uint8_t>& key);
    ~Twofish();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
};

#endif // TWOFISH_H
