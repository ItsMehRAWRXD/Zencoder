// Simple encryption example
// This demonstrates how to use the crypto project for basic file encryption

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>

// Simple XOR encryption for demonstration
// In production, use the proper encryption algorithms from the encryption/ directory
class SimpleEncryptor {
private:
    std::string key;
    
public:
    SimpleEncryptor(const std::string& encryptionKey) : key(encryptionKey) {}
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) {
        std::vector<uint8_t> encrypted = data;
        for (size_t i = 0; i < encrypted.size(); i++) {
            encrypted[i] ^= key[i % key.length()];
        }
        return encrypted;
    }
    
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) {
        // XOR encryption is symmetric
        return encrypt(data);
    }
};

// Helper function to read file
std::vector<uint8_t> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    
    return buffer;
}

// Helper function to write file
void writeFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    std::cout << "=== Simple Encryption Example ===" << std::endl;
    std::cout << "This example demonstrates basic file encryption." << std::endl << std::endl;
    
    try {
        // Get input from user
        std::string inputFile, outputFile, key;
        int choice;
        
        std::cout << "Select operation:" << std::endl;
        std::cout << "1. Encrypt file" << std::endl;
        std::cout << "2. Decrypt file" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear newline
        
        std::cout << "Enter input file path: ";
        std::getline(std::cin, inputFile);
        
        std::cout << "Enter output file path: ";
        std::getline(std::cin, outputFile);
        
        std::cout << "Enter encryption key: ";
        std::getline(std::cin, key);
        
        if (key.empty()) {
            std::cerr << "Error: Key cannot be empty!" << std::endl;
            return 1;
        }
        
        // Create encryptor
        SimpleEncryptor encryptor(key);
        
        // Read input file
        std::vector<uint8_t> inputData = readFile(inputFile);
        std::cout << "Read " << inputData.size() << " bytes from input file." << std::endl;
        
        // Process data
        std::vector<uint8_t> outputData;
        if (choice == 1) {
            outputData = encryptor.encrypt(inputData);
            std::cout << "File encrypted successfully!" << std::endl;
        } else if (choice == 2) {
            outputData = encryptor.decrypt(inputData);
            std::cout << "File decrypted successfully!" << std::endl;
        } else {
            std::cerr << "Invalid choice!" << std::endl;
            return 1;
        }
        
        // Write output file
        writeFile(outputFile, outputData);
        std::cout << "Written " << outputData.size() << " bytes to output file." << std::endl;
        
        std::cout << "\nOperation completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}