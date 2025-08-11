#ifndef GCM_H
#define GCM_H

#include <vector>
#include <cstdint>

class GCM {
public:
    GCM(const std::vector<uint8_t>& key);
    ~GCM();
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& aad);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& aad);

private:
    std::vector<uint8_t> key;
};

#endif // GCM_H
