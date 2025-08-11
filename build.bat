@echo off
setlocal enabledelayedexpansion

REM Zencoder Build Script for Windows
REM This script builds the Zencoder project with Visual Studio 2022

echo ========================================
echo    Zencoder Build Script for Windows
echo ========================================

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo [ERROR] CMakeLists.txt not found. Please run this script from the project root directory.
    pause
    exit /b 1
)

REM Default settings
set BUILD_TYPE=Release
set BUILD_DIR=build
set CLEAN_BUILD=false
set INSTALL=false
set VS_VERSION=2022
set PLATFORM=x64

REM Parse command line arguments
:parse_args
if "%~1"=="" goto :end_parse
if "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :parse_args
)
if "%~1"=="--clean" (
    set CLEAN_BUILD=true
    shift
    goto :parse_args
)
if "%~1"=="--install" (
    set INSTALL=true
    shift
    goto :parse_args
)
if "%~1"=="--x86" (
    set PLATFORM=x86
    shift
    goto :parse_args
)
if "%~1"=="--help" (
    echo Usage: %0 [OPTIONS]
    echo Options:
    echo   --debug      Build in Debug mode (default: Release)
    echo   --clean      Clean build directory before building
    echo   --install    Install after building
    echo   --x86        Build for x86 platform (default: x64)
    echo   --help       Show this help message
    pause
    exit /b 0
)
shift
goto :parse_args
:end_parse

echo [INFO] Building Zencoder in %BUILD_TYPE% mode for %PLATFORM%

REM Check for required tools
echo [INFO] Checking dependencies...

REM Check for CMake
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found. Please install CMake and add it to your PATH.
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

for /f "tokens=3" %%i in ('cmake --version 2^>^&1 ^| findstr /r "cmake version"') do set CMAKE_VERSION=%%i
echo [SUCCESS] CMake %CMAKE_VERSION% found

REM Check for Visual Studio
if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    for /f "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set VS_PATH=%%i
    if defined VS_PATH (
        echo [SUCCESS] Visual Studio found at: %VS_PATH%
    ) else (
        echo [WARNING] Visual Studio 2022 with C++ tools not found
    )
) else (
    echo [WARNING] Visual Studio installer not found
)

REM Check for OpenSSL
set OPENSSL_FOUND=false
if exist "C:\OpenSSL-Win64\include\openssl\ssl.h" (
    set OPENSSL_PATH=C:\OpenSSL-Win64
    set OPENSSL_FOUND=true
) else if exist "C:\OpenSSL-Win32\include\openssl\ssl.h" (
    set OPENSSL_PATH=C:\OpenSSL-Win32
    set OPENSSL_FOUND=true
) else if exist "C:\Program Files\OpenSSL-Win64\include\openssl\ssl.h" (
    set OPENSSL_PATH=C:\Program Files\OpenSSL-Win64
    set OPENSSL_FOUND=true
) else if exist "C:\vcpkg\installed\%PLATFORM%-windows\include\openssl\ssl.h" (
    set OPENSSL_PATH=C:\vcpkg\installed\%PLATFORM%-windows
    set OPENSSL_FOUND=true
)

if "%OPENSSL_FOUND%"=="true" (
    echo [SUCCESS] OpenSSL found at: %OPENSSL_PATH%
) else (
    echo [WARNING] OpenSSL not found in common locations
    echo Please install OpenSSL for Windows or use vcpkg:
    echo Download from: https://slproweb.com/products/Win32OpenSSL.html
    echo Or use vcpkg: vcpkg install openssl
)

REM Clean build directory if requested
if "%CLEAN_BUILD%"=="true" (
    echo [INFO] Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)

REM Create build directory
echo [INFO] Creating build directory...
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

REM Configure with CMake
echo [INFO] Configuring with CMake...
set CMAKE_ARGS=-DCMAKE_BUILD_TYPE=%BUILD_TYPE%

if "%OPENSSL_FOUND%"=="true" (
    set CMAKE_ARGS=%CMAKE_ARGS% -DOPENSSL_ROOT_DIR=%OPENSSL_PATH%
)

if "%PLATFORM%"=="x86" (
    set CMAKE_ARGS=%CMAKE_ARGS% -A Win32
) else (
    set CMAKE_ARGS=%CMAKE_ARGS% -A x64
)

cmake .. %CMAKE_ARGS%

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

REM Build the project
echo [INFO] Building project...
cmake --build . --config %BUILD_TYPE% --parallel

if %errorlevel% neq 0 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully!

REM Show build results
if exist "bin\%BUILD_TYPE%\zencoder.exe" (
    echo [SUCCESS] Executable created: bin\%BUILD_TYPE%\zencoder.exe
    dir "bin\%BUILD_TYPE%\zencoder.exe"
) else (
    echo [WARNING] Executable not found in expected location
)

REM Install if requested
if "%INSTALL%"=="true" (
    echo [INFO] Installing...
    cmake --install . --config %BUILD_TYPE%
    if %errorlevel% equ 0 (
        echo [SUCCESS] Installation completed
    ) else (
        echo [ERROR] Installation failed
    )
)

REM Go back to project root
cd ..

echo [SUCCESS] Build process completed!
echo [INFO] You can now run the application with: %BUILD_DIR%\bin\%BUILD_TYPE%\zencoder.exe

REM Open Visual Studio solution if it exists
if exist "%BUILD_DIR%\Zencoder.sln" (
    echo [INFO] Opening Visual Studio solution...
    start "" "%BUILD_DIR%\Zencoder.sln"
)

pause