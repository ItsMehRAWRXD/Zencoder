#include <iostream>
#include <vector>
#include <string>
#include "../src/encryption/aes_cbc.h"
#include "../src/utils/file_utils.h"

int main() {
    std::cout << "=== Zencoder Simple Test ===" << std::endl;
    
    // Test file utilities
    std::cout << "Testing file utilities..." << std::endl;
    
    // Create a test file
    std::string testContent = "Hello, this is a test file for Zencoder!";
    std::string testFile = "test_file.txt";
    
    if (FileUtils::writeFile(testFile, testContent)) {
        std::cout << "✓ Test file created successfully" << std::endl;
    } else {
        std::cout << "✗ Failed to create test file" << std::endl;
        return 1;
    }
    
    // Read the test file
    auto readContent = FileUtils::readFile(testFile);
    if (!readContent.empty()) {
        std::string content(readContent.begin(), readContent.end());
        if (content == testContent) {
            std::cout << "✓ File read successfully" << std::endl;
        } else {
            std::cout << "✗ File content mismatch" << std::endl;
            return 1;
        }
    } else {
        std::cout << "✗ Failed to read test file" << std::endl;
        return 1;
    }
    
    // Test AES encryption
    std::cout << "Testing AES-CBC encryption..." << std::endl;
    
    AESCBC aes;
    
    // Generate key and IV
    auto key = AESCBC::generateKey(32);  // 256-bit key
    auto iv = AESCBC::generateIV();
    
    if (key.empty() || iv.empty()) {
        std::cout << "✗ Failed to generate key or IV" << std::endl;
        return 1;
    }
    
    std::cout << "✓ Key and IV generated successfully" << std::endl;
    
    // Initialize AES
    if (!aes.initialize(key, iv)) {
        std::cout << "✗ Failed to initialize AES" << std::endl;
        return 1;
    }
    
    std::cout << "✓ AES initialized successfully" << std::endl;
    
    // Encrypt the test content
    std::vector<uint8_t> plaintext(testContent.begin(), testContent.end());
    auto ciphertext = aes.encrypt(plaintext);
    
    if (ciphertext.empty()) {
        std::cout << "✗ Encryption failed" << std::endl;
        return 1;
    }
    
    std::cout << "✓ Encryption successful" << std::endl;
    
    // Decrypt the ciphertext
    auto decrypted = aes.decrypt(ciphertext);
    
    if (decrypted.empty()) {
        std::cout << "✗ Decryption failed" << std::endl;
        return 1;
    }
    
    std::string decryptedString(decrypted.begin(), decrypted.end());
    if (decryptedString == testContent) {
        std::cout << "✓ Decryption successful - content matches" << std::endl;
    } else {
        std::cout << "✗ Decryption failed - content mismatch" << std::endl;
        return 1;
    }
    
    // Clean up
    FileUtils::deleteFile(testFile);
    std::cout << "✓ Test file cleaned up" << std::endl;
    
    std::cout << "\n=== All tests passed! ===" << std::endl;
    return 0;
}