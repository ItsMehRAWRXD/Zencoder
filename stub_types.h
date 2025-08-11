#ifndef STUB_TYPES_H
#define STUB_TYPES_H

#include <string>
#include <vector>
#include <cstdint>
#include <map>

// Enum for different stub/exploit payload types
enum class StubType {
    // Office document exploits
    XLL_EXPLOIT,        // Excel Add-in (.xll) exploit
    DOC_XLS_EXPLOIT,    // Excel document (.xls) exploit
    DOCX_EXPLOIT,       // Word document (.docx) exploit
    RTF_EXPLOIT,        // Rich Text Format (.rtf) exploit
    
    // PDF exploits
    PDF_EXPLOIT,        // PDF document exploit
    
    // Script-based exploits
    VBS_EXPLOIT,        // Visual Basic Script exploit
    JS_EXPLOIT,         // JavaScript exploit
    PS1_EXPLOIT,        // PowerShell script exploit
    
    // Binary exploits
    EXE_EXPLOIT,        // Executable file exploit
    DLL_EXPLOIT,        // Dynamic Link Library exploit
    
    // Archive exploits
    ZIP_EXPLOIT,        // ZIP archive exploit
    RAR_EXPLOIT,        // RAR archive exploit
    
    // Other formats
    HTA_EXPLOIT,        // HTML Application exploit
    ISO_EXPLOIT,        // ISO image exploit
    LNK_EXPLOIT,        // Windows shortcut exploit
    
    // Generic/Custom
    CUSTOM_EXPLOIT      // Custom payload type
};

// Function prototype for generating exploit payloads
// Parameters:
//   stubType - The type of exploit payload to generate
//   payload - The actual payload data to embed
//   outputPath - Where to save the generated exploit file
//   options - Additional configuration options (optional)
// Returns: true if successful, false otherwise
bool generate_exploit_payload(
    StubType stubType,
    const std::vector<uint8_t>& payload,
    const std::string& outputPath,
    const std::map<std::string, std::string>& options = {}
);

// Helper function to get stub type description
std::string get_stub_type_description(StubType stubType);

// Helper function to get file extension for stub type
std::string get_stub_type_extension(StubType stubType);

#endif // STUB_TYPES_H