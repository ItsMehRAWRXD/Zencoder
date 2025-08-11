#include "pe_parser.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <random>

bool PEParser::loadPE(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        return false;
    }
    
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    peData.resize(size);
    file.read(reinterpret_cast<char*>(peData.data()), size);
    
    return loadPE(peData);
}

bool PEParser::loadPE(const std::vector<uint8_t>& data) {
    peData = data;
    
    if (peData.size() < sizeof(IMAGE_DOS_HEADER)) {
        return false;
    }
    
    dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(peData.data());
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
        return false;
    }
    
    if (peData.size() < dosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS)) {
        return false;
    }
    
    ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(peData.data() + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
        return false;
    }
    
    sectionHeaders = reinterpret_cast<IMAGE_SECTION_HEADER*>(
        reinterpret_cast<uint8_t*>(ntHeaders) + sizeof(IMAGE_NT_HEADERS)
    );
    
    return true;
}

IMAGE_DOS_HEADER* PEParser::getDOSHeader() {
    return dosHeader;
}

IMAGE_NT_HEADERS* PEParser::getNTHeaders() {
    return ntHeaders;
}

IMAGE_SECTION_HEADER* PEParser::getSectionHeaders() {
    return sectionHeaders;
}

uint32_t PEParser::rvaToFileOffset(uint32_t rva) {
    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++) {
        if (rva >= sectionHeaders[i].VirtualAddress &&
            rva < sectionHeaders[i].VirtualAddress + sectionHeaders[i].SizeOfRawData) {
            return rva - sectionHeaders[i].VirtualAddress + sectionHeaders[i].PointerToRawData;
        }
    }
    return 0;
}

uint8_t* PEParser::rvaToPointer(uint32_t rva) {
    uint32_t offset = rvaToFileOffset(rva);
    if (offset == 0 || offset >= peData.size()) {
        return nullptr;
    }
    return peData.data() + offset;
}

std::string PEParser::readStringFromRVA(uint32_t rva) {
    uint8_t* ptr = rvaToPointer(rva);
    if (!ptr) {
        return "";
    }
    
    return std::string(reinterpret_cast<char*>(ptr));
}

std::vector<PEParser::ImportDescriptor> PEParser::getImports() {
    std::vector<ImportDescriptor> imports;
    
    auto& importDir = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir.VirtualAddress == 0 || importDir.Size == 0) {
        return imports;
    }
    
    auto* importDesc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(
        rvaToPointer(importDir.VirtualAddress)
    );
    
    if (!importDesc) {
        return imports;
    }
    
    while (importDesc->Name != 0) {
        ImportDescriptor desc;
        desc.dllName = readStringFromRVA(importDesc->Name);
        desc.originalFirstThunk = importDesc->OriginalFirstThunk;
        desc.firstThunk = importDesc->FirstThunk;
        
        // Read function names
        auto* thunk = reinterpret_cast<IMAGE_THUNK_DATA*>(
            rvaToPointer(importDesc->OriginalFirstThunk ? importDesc->OriginalFirstThunk : importDesc->FirstThunk)
        );
        
        if (thunk) {
            while (thunk->u1.AddressOfData != 0) {
                if (!(thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)) {
                    auto* importByName = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(
                        rvaToPointer(thunk->u1.AddressOfData)
                    );
                    if (importByName) {
                        desc.functions.push_back(std::string(reinterpret_cast<char*>(importByName->Name)));
                    }
                }
                thunk++;
            }
        }
        
        imports.push_back(desc);
        importDesc++;
    }
    
    return imports;
}

void PEParser::xorEncryptString(char* str, uint8_t key) {
    while (*str) {
        *str ^= key;
        str++;
    }
}

bool PEParser::obfuscateImports() {
    auto& importDir = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir.VirtualAddress == 0 || importDir.Size == 0) {
        return false;
    }
    
    auto* importDesc = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(
        rvaToPointer(importDir.VirtualAddress)
    );
    
    if (!importDesc) {
        return false;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 255);
    
    // Obfuscate DLL names
    while (importDesc->Name != 0) {
        char* dllName = reinterpret_cast<char*>(rvaToPointer(importDesc->Name));
        if (dllName) {
            // Simple XOR obfuscation - in production, use stronger methods
            uint8_t key = dis(gen);
            
            // Store the key in an unused field or create a decoder
            // For now, we'll use a reversible transformation
            std::string original(dllName);
            
            // ROT13-style transformation for ASCII letters
            for (char& c : original) {
                if ((c >= 'A' && c <= 'Z')) {
                    c = ((c - 'A' + 13) % 26) + 'A';
                } else if ((c >= 'a' && c <= 'z')) {
                    c = ((c - 'a' + 13) % 26) + 'a';
                }
            }
            
            strcpy(dllName, original.c_str());
        }
        
        importDesc++;
    }
    
    return true;
}

bool PEParser::hideImport(const std::string& dllName, const std::string& functionName) {
    // This would implement import hiding by:
    // 1. Removing the import from the import table
    // 2. Resolving it dynamically at runtime
    // 3. Patching call sites
    
    // For now, return true as a placeholder
    return true;
}

std::vector<PEParser::ExportDescriptor> PEParser::getExports() {
    std::vector<ExportDescriptor> exports;
    
    auto& exportDir = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (exportDir.VirtualAddress == 0 || exportDir.Size == 0) {
        return exports;
    }
    
    auto* exportDesc = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
        rvaToPointer(exportDir.VirtualAddress)
    );
    
    if (!exportDesc) {
        return exports;
    }
    
    ExportDescriptor desc;
    desc.dllName = readStringFromRVA(exportDesc->Name);
    desc.addressOfFunctions = exportDesc->AddressOfFunctions;
    desc.addressOfNames = exportDesc->AddressOfNames;
    desc.addressOfOrdinals = exportDesc->AddressOfNameOrdinals;
    
    // Read function names
    auto* names = reinterpret_cast<uint32_t*>(rvaToPointer(exportDesc->AddressOfNames));
    auto* ordinals = reinterpret_cast<uint16_t*>(rvaToPointer(exportDesc->AddressOfNameOrdinals));
    
    if (names && ordinals) {
        for (uint32_t i = 0; i < exportDesc->NumberOfNames; i++) {
            desc.functions.push_back(readStringFromRVA(names[i]));
            desc.ordinals.push_back(ordinals[i]);
        }
    }
    
    exports.push_back(desc);
    return exports;
}

bool PEParser::obfuscateExports() {
    auto& exportDir = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (exportDir.VirtualAddress == 0 || exportDir.Size == 0) {
        return false;
    }
    
    auto* exportDesc = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
        rvaToPointer(exportDir.VirtualAddress)
    );
    
    if (!exportDesc) {
        return false;
    }
    
    // Obfuscate export names
    auto* names = reinterpret_cast<uint32_t*>(rvaToPointer(exportDesc->AddressOfNames));
    
    if (names) {
        for (uint32_t i = 0; i < exportDesc->NumberOfNames; i++) {
            char* funcName = reinterpret_cast<char*>(rvaToPointer(names[i]));
            if (funcName) {
                // Apply obfuscation
                std::string original(funcName);
                
                // Simple character substitution
                for (char& c : original) {
                    if ((c >= 'A' && c <= 'Z')) {
                        c = ((c - 'A' + 7) % 26) + 'A';
                    } else if ((c >= 'a' && c <= 'z')) {
                        c = ((c - 'a' + 7) % 26) + 'a';
                    }
                }
                
                strcpy(funcName, original.c_str());
            }
        }
    }
    
    return true;
}

IMAGE_SECTION_HEADER* PEParser::findSection(const std::string& name) {
    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++) {
        char sectionName[9] = {0};
        memcpy(sectionName, sectionHeaders[i].Name, 8);
        if (name == sectionName) {
            return &sectionHeaders[i];
        }
    }
    return nullptr;
}

bool PEParser::addSection(const std::string& name, const std::vector<uint8_t>& data, DWORD characteristics) {
    // Check if we can add another section
    if (ntHeaders->FileHeader.NumberOfSections >= 96) { // PE format limit
        return false;
    }
    
    // Find the last section
    int lastSectionIdx = ntHeaders->FileHeader.NumberOfSections - 1;
    auto& lastSection = sectionHeaders[lastSectionIdx];
    
    // Calculate new section parameters
    uint32_t fileAlignment = ntHeaders->OptionalHeader.FileAlignment;
    uint32_t sectionAlignment = ntHeaders->OptionalHeader.SectionAlignment;
    
    // New section header location
    auto* newSection = &sectionHeaders[ntHeaders->FileHeader.NumberOfSections];
    
    // Initialize new section
    memset(newSection, 0, sizeof(IMAGE_SECTION_HEADER));
    strncpy(reinterpret_cast<char*>(newSection->Name), name.c_str(), 8);
    
    // Set virtual parameters
    newSection->VirtualAddress = lastSection.VirtualAddress + 
        ((lastSection.Misc.VirtualSize + sectionAlignment - 1) / sectionAlignment) * sectionAlignment;
    newSection->Misc.VirtualSize = data.size();
    
    // Set file parameters
    newSection->PointerToRawData = lastSection.PointerToRawData + 
        ((lastSection.SizeOfRawData + fileAlignment - 1) / fileAlignment) * fileAlignment;
    newSection->SizeOfRawData = ((data.size() + fileAlignment - 1) / fileAlignment) * fileAlignment;
    
    // Set characteristics
    newSection->Characteristics = characteristics;
    
    // Update PE headers
    ntHeaders->FileHeader.NumberOfSections++;
    ntHeaders->OptionalHeader.SizeOfImage = newSection->VirtualAddress + 
        ((newSection->Misc.VirtualSize + sectionAlignment - 1) / sectionAlignment) * sectionAlignment;
    
    // Resize PE data to accommodate new section
    size_t oldSize = peData.size();
    size_t newSize = newSection->PointerToRawData + newSection->SizeOfRawData;
    peData.resize(newSize);
    
    // Copy section data
    memcpy(peData.data() + newSection->PointerToRawData, data.data(), data.size());
    
    // Zero-fill padding
    if (newSection->SizeOfRawData > data.size()) {
        memset(peData.data() + newSection->PointerToRawData + data.size(), 0, 
               newSection->SizeOfRawData - data.size());
    }
    
    return true;
}

bool PEParser::save(const std::string& filePath) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(peData.data()), peData.size());
    return file.good();
}