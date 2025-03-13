<p align="center">
  <img src="assets/app_icon.png" alt="Keplemeyen Helper Logo" width="128" height="128">
</p>

<h1 align="center">Keplemeyen Helper</h1>

A modern C++ application designed to assist game support agents in analyzing player logs and providing efficient support responses. Originally developed in Python, this program has been completely rewritten in C++ to provide better flexibility and performance.

## Version
Version: 0.2.0

## Overview
This tool streamlines the support process through various specialized modules that analyze player logs. Based on which analysis module is used, the program generates appropriate response templates that agents can copy and send to players, significantly reducing both investigation and response times.

## Key Features
- Modern Qt-based user interface providing an intuitive user experience
- Efficient log analysis modules for quick problem identification
- Time discrepancy analysis module
- Drag & drop support for CSV and Excel files
- Always-on-top window functionality
- Ready-to-use response templates based on analysis results

## Installation

### Windows
- Download the pre-built installer (`KeplemeyenHelper-0.2.0-win64.exe`) from the [Releases page](https://github.com/ddbeyin/keplemeyen-helper/releases)
- Run the installer and follow the prompts
- The application will be installed to your chosen location and shortcuts will be created on the desktop and in the Start menu

### Linux and macOS
While Keplemeyen Helper should be compatible with Linux and macOS systems due to the cross-platform nature of Qt and C++, it has currently only been tested and verified on Windows 11.

If you wish to use it on Linux or macOS, you'll need to build it from source following the instructions below.

## Building from Source

### Prerequisites

#### Windows
- Qt 6.5 or higher (6.8.2 recommended)
  - Download from [Qt's official website](https://www.qt.io/download) (open source version)
- MinGW compiler compatible with your Qt version
  - Usually located at `C:\Qt\Tools\mingw1310_64\bin` or similar
- CMake 3.19 or higher
  - Either use the one included with Qt (`C:\Qt\Tools\CMake_64\bin`)
  - Or download from [CMake's official website](https://cmake.org/download/)

#### Linux
- Qt 6.5 or higher development packages
- CMake 3.19 or higher
- GCC 11+ or Clang 13+

#### macOS
- Qt 6.5 or higher
- CMake 3.19 or higher
- Xcode Command Line Tools

### Build Instructions

#### Using the build script (Windows)
The easiest way to build on Windows is to use the included build script:

1. Edit `build_installer.bat` to match your Qt and MinGW installation paths
   - Update `QT_DIR`, `MINGW_DIR`, and `CMAKE_DIR` variables
2. Run `build_installer.bat` from Command Prompt or by double-clicking
3. The installer will be created in the `build` directory

#### Manual Build (All Platforms)
1. Clone the repository and create a build directory:
   ```bash
   git clone https://github.com/ddbeyin/keplemeyen-helper.git
   cd keplemeyen-helper
   mkdir build
   cd build
   ```

2. Configure with CMake:
   - Windows with MinGW:
     ```bash
     cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:/Qt/6.8.2/mingw_64
     ```
   - Linux/macOS:
     ```bash
     cmake .. -DCMAKE_BUILD_TYPE=Release
     ```

3. Build the project:
   ```bash
   cmake --build . --config Release
   ```

4. Create the installer package (optional):
   ```bash
   cmake --build . --target package
   ```

## Usage
1. Launch the application
2. Load a log file (CSV or Excel format) either by:
   - Clicking the Load button and selecting a file
   - Dragging and dropping a file onto the application
3. Select an analysis module (currently Time Discrepancy available)
4. Click Analyze to process the file
5. Review results and use the generated response templates

## Troubleshooting

### Common Issues
1. **CMake can't find Qt**
   - Ensure `CMAKE_PREFIX_PATH` points to your Qt installation directory
   - Example: `-DCMAKE_PREFIX_PATH=C:/Qt/6.8.2/mingw_64`

2. **Build fails with compiler errors**
   - Make sure you're using a compatible MinGW version with your Qt installation
   - Use the MinGW provided with Qt, typically located at `C:\Qt\Tools\mingw1310_64\bin`

3. **Application crashes on startup**
   - Ensure all Qt DLLs are available in the application directory
   - Run the windeployqt tool to gather required libraries:
     ```
     C:\Qt\6.8.2\mingw_64\bin\windeployqt.exe path\to\KeplemeyenHelper.exe
     ```

## Contributing
Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute to this project.

## Security
For information about reporting security vulnerabilities, please see [SECURITY.md](SECURITY.md).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact
Developer: ddbeyin
Email: ddbeyin@gmail.com
GitHub: [github.com/ddbeyin/keplemeyen-helper](https://github.com/ddbeyin/keplemeyen-helper)