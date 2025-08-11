#include <iostream>
#include <string>
#include <vector>
#include "../src/encryption/aes_cbc.h"
#include "../src/utils/file_utils.h"

/**
 * Basic Usage Example for Zencoder
 * 
 * This example demonstrates:
 * 1. File encryption and decryption
 * 2. Working with different file types
 * 3. Error handling
 * 4. Secure key management
 */

int main() {
    std::cout << "=== Zencoder Basic Usage Example ===" << std::endl;
    
    // Example 1: Encrypt a text file
    std::cout << "\n1. Encrypting a text file..." << std::endl;
    
    // Create a sample text file
    std::string sampleText = "This is a sample text file that will be encrypted.\n"
                            "It contains multiple lines of text.\n"
                            "The encryption will protect this content.";
    
    std::string inputFile = "sample.txt";
    std::string encryptedFile = "sample.txt.enc";
    std::string decryptedFile = "sample_decrypted.txt";
    
    // Write the sample file
    if (!FileUtils::writeFile(inputFile, sampleText)) {
        std::cout << "Error: Failed to create sample file" << std::endl;
        return 1;
    }
    
    std::cout << "✓ Created sample file: " << inputFile << std::endl;
    
    // Read the file
    auto fileData = FileUtils::readFile(inputFile);
    if (fileData.empty()) {
        std::cout << "Error: Failed to read sample file" << std::endl;
        return 1;
    }
    
    // Initialize AES encryption
    AESCBC aes;
    auto key = AESCBC::generateKey(32);  // 256-bit key
    auto iv = AESCBC::generateIV();
    
    if (!aes.initialize(key, iv)) {
        std::cout << "Error: Failed to initialize AES encryption" << std::endl;
        return 1;
    }
    
    // Encrypt the file data
    auto encryptedData = aes.encrypt(fileData);
    if (encryptedData.empty()) {
        std::cout << "Error: Encryption failed" << std::endl;
        return 1;
    }
    
    // Save encrypted data
    if (!FileUtils::writeFile(encryptedFile, encryptedData)) {
        std::cout << "Error: Failed to save encrypted file" << std::endl;
        return 1;
    }
    
    std::cout << "✓ File encrypted and saved as: " << encryptedFile << std::endl;
    
    // Example 2: Decrypt the file
    std::cout << "\n2. Decrypting the file..." << std::endl;
    
    // Read encrypted file
    auto encryptedFileData = FileUtils::readFile(encryptedFile);
    if (encryptedFileData.empty()) {
        std::cout << "Error: Failed to read encrypted file" << std::endl;
        return 1;
    }
    
    // Decrypt the data
    auto decryptedData = aes.decrypt(encryptedFileData);
    if (decryptedData.empty()) {
        std::cout << "Error: Decryption failed" << std::endl;
        return 1;
    }
    
    // Save decrypted data
    if (!FileUtils::writeFile(decryptedFile, decryptedData)) {
        std::cout << "Error: Failed to save decrypted file" << std::endl;
        return 1;
    }
    
    std::cout << "✓ File decrypted and saved as: " << decryptedFile << std::endl;
    
    // Verify the decryption was successful
    auto verifyData = FileUtils::readFile(decryptedFile);
    std::string verifyText(verifyData.begin(), verifyData.end());
    
    if (verifyText == sampleText) {
        std::cout << "✓ Verification successful: Decrypted content matches original" << std::endl;
    } else {
        std::cout << "✗ Verification failed: Content mismatch" << std::endl;
        return 1;
    }
    
    // Example 3: File information
    std::cout << "\n3. File information..." << std::endl;
    
    std::cout << "Original file size: " << FileUtils::getFileSize(inputFile) << " bytes" << std::endl;
    std::cout << "Encrypted file size: " << FileUtils::getFileSize(encryptedFile) << " bytes" << std::endl;
    std::cout << "File type: " << (FileUtils::isTextFile(inputFile) ? "Text" : "Binary") << std::endl;
    
    // Example 4: Secure deletion
    std::cout << "\n4. Secure deletion..." << std::endl;
    
    // Create a temporary file for secure deletion demo
    std::string tempFile = "temp_secret.txt";
    std::string secretData = "This is secret data that will be securely deleted.";
    FileUtils::writeFile(tempFile, secretData);
    
    std::cout << "✓ Created temporary file for secure deletion demo" << std::endl;
    
    // Securely delete the file
    if (FileUtils::secureDelete(tempFile, 3)) {
        std::cout << "✓ File securely deleted (3 passes)" << std::endl;
    } else {
        std::cout << "✗ Secure deletion failed" << std::endl;
    }
    
    // Clean up example files
    std::cout << "\n5. Cleaning up..." << std::endl;
    
    FileUtils::deleteFile(inputFile);
    FileUtils::deleteFile(encryptedFile);
    FileUtils::deleteFile(decryptedFile);
    
    std::cout << "✓ Cleanup completed" << std::endl;
    
    std::cout << "\n=== Example completed successfully! ===" << std::endl;
    std::cout << "\nKey features demonstrated:" << std::endl;
    std::cout << "- File encryption and decryption" << std::endl;
    std::cout << "- Secure key generation" << std::endl;
    std::cout << "- File utilities" << std::endl;
    std::cout << "- Secure file deletion" << std::endl;
    std::cout << "- Error handling" << std::endl;
    
    return 0;
}