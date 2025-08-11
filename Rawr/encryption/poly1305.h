#ifndef POLY1305_H
#define POLY1305_H

#include <vector>
#include <cstdint>

class Poly1305 {
public:
    Poly1305(const std::vector<uint8_t>& key);
    ~Poly1305();
    
    std::vector<uint8_t> computeTag(const std::vector<uint8_t>& message);
    bool verifyTag(const std::vector<uint8_t>& message, const std::vector<uint8_t>& tag);

private:
    std::vector<uint8_t> key;
};

#endif // POLY1305_H
