#pragma once

#include <cstdint>

// Encryption algorithm enumerations
enum class EncryptionAlgorithm : uint8_t {
    NONE = 0,
    AES_CBC = 1,
    AES_CTR = 2,
    CHACHA20 = 3,
    BLOWFISH = 4,
    TWOFISH = 5,
    SERPENT = 6,
    CAMELLIA = 7,
    GCM = 8,
    POLY1305 = 9,
    RC4 = 10
};

// Operation modes
enum class OperationMode : uint8_t {
    ENCRYPT = 0,
    DECRYPT = 1,
    PACK = 2,
    UNPACK = 3
};

// File types
enum class FileType : uint8_t {
    UNKNOWN = 0,
    BINARY = 1,
    TEXT = 2,
    PE_EXECUTABLE = 3,
    ARCHIVE = 4
};

// Security levels
enum class SecurityLevel : uint8_t {
    LOW = 0,
    MEDIUM = 1,
    HIGH = 2,
    MAXIMUM = 3
};

// Memory protection types
enum class MemoryProtection : uint8_t {
    NONE = 0,
    READ_ONLY = 1,
    WRITE_ONLY = 2,
    READ_WRITE = 3,
    EXECUTE = 4,
    EXECUTE_READ = 5,
    EXECUTE_READ_WRITE = 6
};

// Error codes
enum class ErrorCode : uint32_t {
    SUCCESS = 0,
    FILE_NOT_FOUND = 1,
    PERMISSION_DENIED = 2,
    INVALID_KEY = 3,
    ENCRYPTION_FAILED = 4,
    DECRYPTION_FAILED = 5,
    INVALID_FORMAT = 6,
    MEMORY_ERROR = 7,
    UNSUPPORTED_ALGORITHM = 8,
    INVALID_PARAMETER = 9,
    IO_ERROR = 10,
    UNKNOWN_ERROR = 0xFFFFFFFF
};

// Status codes
enum class StatusCode : uint8_t {
    OK = 0,
    PENDING = 1,
    FAILED = 2,
    CANCELLED = 3,
    TIMEOUT = 4
};