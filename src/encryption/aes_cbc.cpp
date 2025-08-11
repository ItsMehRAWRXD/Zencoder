#include "aes_cbc.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <stdexcept>
#include <algorithm>
#include <cstring>

class AESCBC::Impl {
public:
    Impl() : ctx(nullptr), initialized(false), keySize(32), blockSize(16) {
        ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create OpenSSL cipher context");
        }
    }
    
    ~Impl() {
        clear();
        if (ctx) {
            EVP_CIPHER_CTX_free(ctx);
        }
    }
    
    bool initialize(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv) {
        if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
            return false;
        }
        if (iv.size() != 16) {
            return false;
        }
        
        this->key = key;
        this->iv = iv;
        this->keySize = key.size();
        this->initialized = true;
        
        return true;
    }
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext) {
        if (!initialized || plaintext.empty()) {
            return {};
        }
        
        const EVP_CIPHER* cipher = nullptr;
        switch (keySize) {
            case 16: cipher = EVP_aes_128_cbc(); break;
            case 24: cipher = EVP_aes_192_cbc(); break;
            case 32: cipher = EVP_aes_256_cbc(); break;
            default: return {};
        }
        
        if (!EVP_EncryptInit_ex(ctx, cipher, nullptr, key.data(), iv.data())) {
            return {};
        }
        
        std::vector<uint8_t> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
        int outLen = 0;
        
        if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &outLen, plaintext.data(), plaintext.size())) {
            return {};
        }
        
        int finalLen = 0;
        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + outLen, &finalLen)) {
            return {};
        }
        
        ciphertext.resize(outLen + finalLen);
        return ciphertext;
    }
    
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext) {
        if (!initialized || ciphertext.empty()) {
            return {};
        }
        
        const EVP_CIPHER* cipher = nullptr;
        switch (keySize) {
            case 16: cipher = EVP_aes_128_cbc(); break;
            case 24: cipher = EVP_aes_192_cbc(); break;
            case 32: cipher = EVP_aes_256_cbc(); break;
            default: return {};
        }
        
        if (!EVP_DecryptInit_ex(ctx, cipher, nullptr, key.data(), iv.data())) {
            return {};
        }
        
        std::vector<uint8_t> plaintext(ciphertext.size());
        int outLen = 0;
        
        if (!EVP_DecryptUpdate(ctx, plaintext.data(), &outLen, ciphertext.data(), ciphertext.size())) {
            return {};
        }
        
        int finalLen = 0;
        if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + outLen, &finalLen)) {
            return {};
        }
        
        plaintext.resize(outLen + finalLen);
        return plaintext;
    }
    
    void clear() {
        if (!key.empty()) {
            std::fill(key.begin(), key.end(), 0);
            key.clear();
        }
        if (!iv.empty()) {
            std::fill(iv.begin(), iv.end(), 0);
            iv.clear();
        }
        initialized = false;
    }
    
    size_t getKeySize() const { return keySize; }
    size_t getBlockSize() const { return blockSize; }
    bool isInitialized() const { return initialized; }
    
private:
    EVP_CIPHER_CTX* ctx;
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;
    bool initialized;
    size_t keySize;
    size_t blockSize;
};

// AESCBC implementation
AESCBC::AESCBC() : pImpl(new Impl()) {}

AESCBC::~AESCBC() {
    delete pImpl;
}

bool AESCBC::initialize(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv) {
    return pImpl->initialize(key, iv);
}

std::vector<uint8_t> AESCBC::encrypt(const std::vector<uint8_t>& plaintext) {
    return pImpl->encrypt(plaintext);
}

std::vector<uint8_t> AESCBC::decrypt(const std::vector<uint8_t>& ciphertext) {
    return pImpl->decrypt(ciphertext);
}

std::vector<uint8_t> AESCBC::generateKey(size_t keySize) {
    if (keySize != 16 && keySize != 24 && keySize != 32) {
        keySize = 32; // Default to 256-bit
    }
    
    std::vector<uint8_t> key(keySize);
    if (RAND_bytes(key.data(), keySize) != 1) {
        return {};
    }
    return key;
}

std::vector<uint8_t> AESCBC::generateIV() {
    std::vector<uint8_t> iv(16);
    if (RAND_bytes(iv.data(), 16) != 1) {
        return {};
    }
    return iv;
}

size_t AESCBC::getKeySize() const {
    return pImpl->getKeySize();
}

size_t AESCBC::getBlockSize() const {
    return pImpl->getBlockSize();
}

bool AESCBC::isInitialized() const {
    return pImpl->isInitialized();
}

void AESCBC::clear() {
    pImpl->clear();
}