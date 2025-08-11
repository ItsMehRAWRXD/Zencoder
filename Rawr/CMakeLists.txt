cmake_minimum_required(VERSION 3.16)
project(Rawr VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/encryption
    ${CMAKE_CURRENT_SOURCE_DIR}/types
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/driver
)

# Collect source files
file(GLOB_RECURSE ENCRYPTION_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/encryption/*.cpp"
)

file(GLOB_RECURSE UTILS_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
)

file(GLOB_RECURSE TYPES_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/types/*.cpp"
)

file(GLOB_RECURSE SRC_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Main executable
add_executable(rawr
    main.cpp
    menu.cpp
    packer.cpp
    ${ENCRYPTION_SOURCES}
    ${UTILS_SOURCES}
    ${TYPES_SOURCES}
    ${SRC_SOURCES}
)

# Simple executable (if using simple_main.cpp)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/simple_main.cpp")
    add_executable(rawr_simple
        simple_main.cpp
        ${ENCRYPTION_SOURCES}
        ${UTILS_SOURCES}
    )
endif()

# Example executable
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/simple_encryption.cpp")
    add_executable(simple_encryption
        examples/simple_encryption.cpp
    )
endif()

# Platform-specific settings
if(WIN32)
    target_compile_definitions(rawr PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(rawr PRIVATE ws2_32 crypt32)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(rawr PRIVATE /W4)
else()
    target_compile_options(rawr PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation rules
install(TARGETS rawr
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# CPack configuration for creating packages
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)