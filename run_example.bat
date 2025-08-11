@echo off
echo ========================================
echo    Zencoder Example Runner for Windows
echo ========================================

REM Check if build directory exists
if not exist "build\bin\Release\zencoder_example.exe" (
    echo [ERROR] Example executable not found. Please build the project first.
    echo Run: build.bat
    pause
    exit /b 1
)

echo [INFO] Running Zencoder example...
echo.

REM Run the example executable
build\bin\Release\zencoder_example.exe

REM Check exit code
if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Example completed successfully!
) else (
    echo.
    echo [ERROR] Example failed!
    pause
    exit /b 1
)

echo.
echo [INFO] Example execution completed.
pause