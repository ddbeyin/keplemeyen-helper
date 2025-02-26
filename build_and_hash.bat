@echo off
setlocal enabledelayedexpansion

echo ===== Keplemeyen Helper Build Script =====

REM Check for required tools
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake 3.19 or later
    exit /b 1
)

where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ not found in PATH. MinGW must be properly installed.
    exit /b 1
)

where mingw32-make >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: mingw32-make not found in PATH. MinGW must be properly installed.
    exit /b 1
)

where python >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Python not found in PATH
    echo Please install Python 3.8 or later
    exit /b 1
)

echo All required tools found.

REM Check if Qt is properly set up
if "%Qt6_DIR%"=="" (
    echo ERROR: Qt6_DIR environment variable not set or empty.
    echo Please set Qt6_DIR to point to your Qt CMake directory,
    echo For example: C:\Qt\6.8.2\mingw_64\lib\cmake\Qt6
    exit /b 1
)

echo Using Qt from: %Qt6_DIR%

REM Create build directory
if not exist build (
    mkdir build
    echo Created build directory
) else (
    echo Using existing build directory
)

REM Configure and build with CMake
echo.
echo === Configuring with CMake ===
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake configuration failed
    cd ..
    exit /b 1
)

echo.
echo === Building with CMake ===
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Build failed
    cd ..
    exit /b 1
)

echo.
echo === Generating Documentation ===
cmake --build . --target docs
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: Documentation generation failed
    echo Continuing with package creation...
) else (
    echo Documentation generated successfully in build/docs/
    
    REM Check if documentation was generated and open it
    if exist docs\html\index.html (
        echo Opening documentation...
        start "" "docs\html\index.html"
    ) else (
        echo WARNING: Documentation files not found at expected location
    )
)

echo.
echo === Creating package ===
cpack -G ZIP
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: Package creation failed
    echo Continuing with hash generation...
) else (
    echo Package created successfully
)

echo.
echo === Generating verification files ===
python ..\generate_hashes.py .
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Hash generation failed
    cd ..
    exit /b 1
)

cd ..
echo.
echo ===== Build completed successfully =====
echo Release files and verification hashes are in the build directory

echo.
echo === Next steps ===
echo 1. Verify the build with: python verify.py
echo 2. Test the application in build/bin
echo 3. If ready to release, run bump_version.bat to update version

exit /b 0