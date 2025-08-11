#ifndef PE_PARSER_H
#define PE_PARSER_H

#include <vector>
#include <string>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
#include "pe_defs.h"
#endif

// PE parsing utilities
class PEParser {
public:
    struct ImportDescriptor {
        std::string dllName;
        std::vector<std::string> functions;
        uint32_t originalFirstThunk;
        uint32_t firstThunk;
    };

    struct ExportDescriptor {
        std::string dllName;
        std::vector<std::string> functions;
        std::vector<uint16_t> ordinals;
        uint32_t addressOfFunctions;
        uint32_t addressOfNames;
        uint32_t addressOfOrdinals;
    };

    // Load and parse PE file
    bool loadPE(const std::string& filePath);
    bool loadPE(const std::vector<uint8_t>& data);
    
    // Get PE headers
    IMAGE_DOS_HEADER* getDOSHeader();
    IMAGE_NT_HEADERS* getNTHeaders();
    IMAGE_SECTION_HEADER* getSectionHeaders();
    
    // Import table operations
    std::vector<ImportDescriptor> getImports();
    bool obfuscateImports();
    bool hideImport(const std::string& dllName, const std::string& functionName);
    
    // Export table operations
    std::vector<ExportDescriptor> getExports();
    bool obfuscateExports();
    
    // Section operations
    IMAGE_SECTION_HEADER* findSection(const std::string& name);
    bool addSection(const std::string& name, const std::vector<uint8_t>& data, DWORD characteristics);
    
    // Save modified PE
    std::vector<uint8_t> getData() const { return peData; }
    bool save(const std::string& filePath);
    
private:
    std::vector<uint8_t> peData;
    IMAGE_DOS_HEADER* dosHeader = nullptr;
    IMAGE_NT_HEADERS* ntHeaders = nullptr;
    IMAGE_SECTION_HEADER* sectionHeaders = nullptr;
    
    // Helper functions
    uint32_t rvaToFileOffset(uint32_t rva);
    uint8_t* rvaToPointer(uint32_t rva);
    std::string readStringFromRVA(uint32_t rva);
    void xorEncryptString(char* str, uint8_t key);
};

#endif // PE_PARSER_H