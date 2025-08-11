#ifndef RC4_H
#define RC4_H

#include <vector>
#include <cstdint>

class RC4 {
public:
    RC4(const std::vector<uint8_t>& key);
    ~RC4();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);

private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> S;
    int i, j;
};

#endif // RC4_H
