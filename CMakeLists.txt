cmake_minimum_required(VERSION 3.16)
project(Zencoder VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Windows-specific settings
if(WIN32)
    # Set Windows SDK version if not specified
    if(NOT CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
        set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION "10.0")
    endif()
    
    # Enable Windows features
    add_definitions(-D_WIN32_WINNT=0x0601)  # Windows 7 or later
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    
    # Link Windows libraries
    set(WINDOWS_LIBS ws2_32 crypt32 advapi32)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# If OpenSSL is not found via find_package, try manual paths for Windows
if(NOT OpenSSL_FOUND)
    if(WIN32)
        # Common OpenSSL installation paths on Windows
        set(OPENSSL_POSSIBLE_PATHS
            "C:/OpenSSL-Win64"
            "C:/OpenSSL-Win32"
            "C:/Program Files/OpenSSL-Win64"
            "C:/Program Files/OpenSSL-Win32"
            "C:/vcpkg/installed/x64-windows"
            "C:/vcpkg/installed/x86-windows"
        )
        
        foreach(OPENSSL_PATH ${OPENSSL_POSSIBLE_PATHS})
            if(EXISTS "${OPENSSL_PATH}/include/openssl/ssl.h")
                set(OPENSSL_ROOT_DIR ${OPENSSL_PATH})
                set(OPENSSL_INCLUDE_DIR "${OPENSSL_PATH}/include")
                set(OPENSSL_LIBRARIES "${OPENSSL_PATH}/lib")
                break()
            endif()
        endforeach()
        
        if(OPENSSL_ROOT_DIR)
            message(STATUS "Found OpenSSL at: ${OPENSSL_ROOT_DIR}")
        else()
            message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL for Windows or use vcpkg.")
        endif()
    endif()
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/menu.cpp
    src/packer.cpp
    src/encryption/aes_cbc.cpp
    src/utils/file_utils.cpp
    src/utils/logging.cpp
)

# Header files
set(HEADERS
    include/packer.h
    src/menu.h
    src/packer.h
    src/encryption/aes_cbc.h
    src/utils/file_utils.h
    src/utils/logging.h
    types/enums.h
)

# Create executable
add_executable(zencoder ${SOURCES} ${HEADERS})

# Link libraries
if(OpenSSL_FOUND)
    target_link_libraries(zencoder OpenSSL::SSL OpenSSL::Crypto)
else()
    target_link_libraries(zencoder ${OPENSSL_LIBRARIES}/libssl.lib ${OPENSSL_LIBRARIES}/libcrypto.lib)
endif()

# Link Windows libraries
if(WIN32)
    target_link_libraries(zencoder ${WINDOWS_LIBS})
endif()

# Compiler-specific settings
if(MSVC)
    target_compile_options(zencoder PRIVATE /W4 /utf-8)
    # Disable specific warnings that are common in Windows development
    target_compile_options(zencoder PRIVATE /wd4996 /wd4267 /wd4244)
else()
    target_compile_options(zencoder PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install rules
install(TARGETS zencoder DESTINATION bin)

# Add test target
enable_testing()
add_executable(zencoder_test test/simple_test.cpp ${SOURCES})
target_link_libraries(zencoder_test zencoder)
add_test(NAME ZencoderTest COMMAND zencoder_test)

# Add example target
add_executable(zencoder_example examples/basic_usage.cpp ${SOURCES})
target_link_libraries(zencoder_example zencoder)