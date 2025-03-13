@echo off
setlocal enabledelayedexpansion

echo ===================================
echo Keplemeyen Helper Installer Builder
echo ===================================
echo.

:: Set up environment for Qt and MinGW (Update these paths to match your installation)
set "QT_DIR=C:\Qt\6.8.2\mingw_64"
set "MINGW_DIR=C:\Qt\Tools\mingw1310_64\bin"
set "CMAKE_DIR=C:\Qt\Tools\CMake_64\bin"

:: Set Qt6_DIR environment variable for CMake
set "Qt6_DIR=%QT_DIR%\lib\cmake\Qt6"

echo Setting up environment:
echo Qt Directory: %QT_DIR%
echo MinGW Directory: %MINGW_DIR%
echo CMake Directory: %CMAKE_DIR%
echo Qt6_DIR: %Qt6_DIR%
echo.

:: Temporarily add to PATH if not already there
set "PATH=%CMAKE_DIR%;%MINGW_DIR%;%PATH%"

:: Check if CMake is available
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake not found in PATH
    echo Please ensure CMake is installed at %CMAKE_DIR%
    exit /b 1
)

:: Create and navigate to build directory
if not exist build (
    echo Creating build directory...
    mkdir build
) else (
    echo Build directory already exists.
)
cd build

:: Configure with CMake
echo.
echo Configuring project with CMake...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=%QT_DIR% ..
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error: CMake configuration failed.
    cd ..
    exit /b 1
)

:: Build the project
echo.
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error: Build failed.
    cd ..
    exit /b 1
)

:: Create the installer package
echo.
echo Generating installer package...
cmake --build . --target package
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error: Package generation failed.
    cd ..
    exit /b 1
)

:: Return to original directory
cd ..

echo.
echo =====================================
echo Build and packaging process complete!
echo =====================================
echo.
echo Installer should be available in the build directory.
echo.

pause