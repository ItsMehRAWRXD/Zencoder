#ifndef WINDOWS_STUBS_H
#define WINDOWS_STUBS_H

#ifndef _WIN32

#include <cstdint>
#include <cstring>

// Windows API stubs for Linux compilation
typedef void* HMODULE;
typedef uint32_t DWORD;

inline HMODULE GetModuleHandleA(const char* moduleName) {
    // Stub implementation
    return nullptr;
}

inline void* GetProcAddress(HMODULE module, const char* procName) {
    // Stub implementation
    return nullptr;
}

inline DWORD GetCurrentProcessId() {
    // Return a dummy process ID
    return 1234;
}

#endif // !_WIN32

#endif // WINDOWS_STUBS_H