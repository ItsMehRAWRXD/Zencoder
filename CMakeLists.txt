cmake_minimum_required(VERSION 3.16)
project(Zencoder VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Find required packages
find_package(OpenSSL REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/menu.cpp
    src/packer.cpp
    src/encryption/aes_cbc.cpp
    src/encryption/aes_ctr.cpp
    src/encryption/blowfish.cpp
    src/encryption/camellia.cpp
    src/encryption/chacha20.cpp
    src/encryption/gcm.cpp
    src/encryption/poly1305.cpp
    src/encryption/rc4.cpp
    src/encryption/serpent.cpp
    src/encryption/twofish.cpp
    src/utils/file_utils.cpp
    src/utils/logging.cpp
)

# Header files
set(HEADERS
    include/packer.h
    src/menu.h
    src/packer.h
    src/encryption/aes_cbc.h
    src/encryption/aes_ctr.h
    src/encryption/blowfish.h
    src/encryption/camellia.h
    src/encryption/chacha20.h
    src/encryption/gcm.h
    src/encryption/poly1305.h
    src/encryption/rc4.h
    src/encryption/serpent.h
    src/encryption/twofish.h
    src/utils/file_utils.h
    src/utils/logging.h
    types/enums.h
)

# Create executable
add_executable(zencoder ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(zencoder OpenSSL::SSL OpenSSL::Crypto)

# Platform-specific settings
if(WIN32)
    target_link_libraries(zencoder ws2_32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

# Compiler-specific settings
if(MSVC)
    target_compile_options(zencoder PRIVATE /W4)
else()
    target_compile_options(zencoder PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS zencoder DESTINATION bin)