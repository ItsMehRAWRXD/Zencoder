# Windows Setup Guide for Visual Studio 2022

This guide will help you set up and build the Zencoder project using Visual Studio 2022 on Windows.

## Prerequisites

### 1. Visual Studio 2022
- Download and install [Visual Studio 2022 Community](https://visualstudio.microsoft.com/downloads/) (free)
- During installation, make sure to select:
  - **Desktop development with C++** workload
  - **CMake tools for Visual Studio** (included in C++ workload)
  - **Windows 10/11 SDK** (latest version)

### 2. CMake
- Download from [cmake.org](https://cmake.org/download/)
- Choose the Windows x64 installer
- During installation, select "Add CMake to the system PATH"
- **Alternative**: CMake is included with Visual Studio 2022

### 3. OpenSSL
You have several options for installing OpenSSL:

#### Option A: Download Pre-built Binaries (Recommended)
1. Go to [https://slproweb.com/products/Win32OpenSSL.html](https://slproweb.com/products/Win32OpenSSL.html)
2. Download **Win64 OpenSSL v3.x.x** (latest stable version)
3. Run the installer
4. Install to `C:\OpenSSL-Win64` (default location)

#### Option B: Use vcpkg (Advanced)
```cmd
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install OpenSSL
.\vcpkg install openssl:x64-windows
```

#### Option C: Build from Source
- Download OpenSSL source from [openssl.org](https://www.openssl.org/source/)
- Follow the build instructions for Windows

## Building the Project

### Method 1: Using the Build Script (Recommended)

1. **Open Command Prompt** as Administrator
2. **Navigate to the project directory**
   ```cmd
   cd C:\path\to\zencoder
   ```
3. **Run the build script**
   ```cmd
   build.bat
   ```
4. **For debug builds**
   ```cmd
   build.bat --debug
   ```
5. **For x86 builds**
   ```cmd
   build.bat --x86
   ```

### Method 2: Using Visual Studio 2022

1. **Open Visual Studio 2022**
2. **Open Folder** (File → Open → Folder)
3. **Select the project directory**
4. **Wait for CMake configuration** (bottom status bar)
5. **Select build configuration** (Debug/Release) from the toolbar
6. **Build** (Build → Build All or Ctrl+Shift+B)

### Method 3: Using CMake GUI

1. **Open CMake GUI**
2. **Set source directory** to the project folder
3. **Set build directory** to `build` (create if needed)
4. **Configure** (select Visual Studio 2022)
5. **Generate**
6. **Open Project** (opens Visual Studio)

### Method 4: Command Line CMake

```cmd
# Create build directory
mkdir build
cd build

# Configure
cmake .. -G "Visual Studio 17 2022" -A x64

# Build
cmake --build . --config Release

# Or open in Visual Studio
start Zencoder.sln
```

## Project Structure in Visual Studio

After opening the project, you'll see:

```
Solution 'Zencoder'
├── zencoder (executable)
├── zencoder_test (test executable)
└── zencoder_example (example executable)
```

## Running the Application

### From Command Line
```cmd
build\bin\Release\zencoder.exe
```

### From Visual Studio
1. Set `zencoder` as startup project (right-click → Set as Startup Project)
2. Press F5 to run with debugging
3. Press Ctrl+F5 to run without debugging

## Troubleshooting

### Common Issues

#### 1. OpenSSL Not Found
**Error**: `OpenSSL not found. Please install OpenSSL for Windows or use vcpkg.`

**Solution**:
- Install OpenSSL using one of the methods above
- Make sure it's installed in a standard location
- Set `OPENSSL_ROOT_DIR` environment variable if needed

#### 2. CMake Configuration Fails
**Error**: `CMake configuration failed`

**Solutions**:
- Make sure Visual Studio 2022 is installed with C++ tools
- Check that CMake is in your PATH
- Try running `build.bat --clean` to clean previous builds

#### 3. Build Errors
**Error**: `fatal error C1083: Cannot open include file: 'openssl/ssl.h'`

**Solutions**:
- Verify OpenSSL is installed correctly
- Check include paths in CMakeLists.txt
- Try setting `OPENSSL_ROOT_DIR` manually

#### 4. Linker Errors
**Error**: `unresolved external symbol`

**Solutions**:
- Make sure OpenSSL libraries are linked correctly
- Check that you're building for the correct platform (x86/x64)
- Verify OpenSSL version compatibility

### Environment Variables

You can set these environment variables to help CMake find dependencies:

```cmd
set OPENSSL_ROOT_DIR=C:\OpenSSL-Win64
set CMAKE_PREFIX_PATH=C:\OpenSSL-Win64
```

### Debugging Tips

1. **Enable verbose output**:
   ```cmd
   cmake --build . --config Debug --verbose
   ```

2. **Check CMake cache**:
   ```cmd
   cmake -L ..  # List all variables
   cmake -LA .. # List all variables with advanced options
   ```

3. **Clean and rebuild**:
   ```cmd
   build.bat --clean
   build.bat
   ```

## Development Workflow

### 1. Making Changes
1. Edit source files in Visual Studio
2. Save changes
3. Build (Ctrl+Shift+B)
4. Test your changes

### 2. Debugging
1. Set breakpoints in your code
2. Press F5 to start debugging
3. Use the debugger to step through code
4. Check variables in the Watch window

### 3. Running Tests
```cmd
# Run tests from command line
build\bin\Release\zencoder_test.exe

# Or from Visual Studio
# Set zencoder_test as startup project and run
```

### 4. Creating Release Builds
```cmd
build.bat --clean
build.bat
```

## IDE Integration

### Visual Studio Features
- **IntelliSense**: Code completion and error detection
- **Debugger**: Full debugging support with breakpoints
- **CMake Integration**: Native CMake support
- **Git Integration**: Source control integration
- **Extensions**: Additional tools and features

### Recommended Extensions
- **C++ IntelliSense**: Enhanced C++ support
- **CMake Tools**: Better CMake integration
- **GitLens**: Enhanced Git integration

## Performance Tips

1. **Use Release builds** for production
2. **Enable parallel builds** (default in Visual Studio)
3. **Use Incremental builds** for development
4. **Clean builds** when switching configurations

## Next Steps

After successful setup:
1. Read the main [README.md](README.md) for project overview
2. Check [examples/basic_usage.cpp](examples/basic_usage.cpp) for usage examples
3. Run tests to verify everything works
4. Start developing your features!

## Support

If you encounter issues:
1. Check this troubleshooting guide
2. Review the main [README.md](README.md)
3. Check the [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md)
4. Create an issue on the project repository