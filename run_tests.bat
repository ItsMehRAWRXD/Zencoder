@echo off
echo ========================================
echo    Zencoder Test Runner for Windows
echo ========================================

REM Check if build directory exists
if not exist "build\bin\Release\zencoder_test.exe" (
    echo [ERROR] Test executable not found. Please build the project first.
    echo Run: build.bat
    pause
    exit /b 1
)

echo [INFO] Running Zencoder tests...
echo.

REM Run the test executable
build\bin\Release\zencoder_test.exe

REM Check exit code
if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] All tests passed!
) else (
    echo.
    echo [ERROR] Some tests failed!
    pause
    exit /b 1
)

echo.
echo [INFO] Test execution completed.
pause