# Keplemeyen Helper

A modern C++ application designed to assist game support agents in analyzing player logs and providing efficient support responses. Originally developed in Python, this program has been completely rewritten in C++ to provide better flexibility and performance.

## Version
Version: 0.2.0 (Development)

## Overview
This tool streamlines the support process through various specialized modules that analyze player logs. Based on which analysis module is used, the program generates appropriate response templates that agents can copy and send to players, significantly reducing both investigation and response times.

## Key Features
- Modern Qt-based user interface providing an intuitive user experience
- Efficient log analysis modules for quick problem identification
- Ready-to-use response templates based on analysis results
- Time discrepancy analysis module
- Drag & drop support for CSV and Excel files
- Always-on-top window functionality
- Modular architecture allowing easy addition of new features

## Installation

### Windows
Download the latest release from the [Releases page](https://github.com/ddbeyin/keplemeyen-helper/releases) and extract it to a location of your choice.

### Linux
Download the DEB package from the [Releases page](https://github.com/ddbeyin/keplemeyen-helper/releases) and install it using:
```bash
sudo dpkg -i KeplemeyenHelper-Linux.deb
```

### macOS
Download the DMG file from the [Releases page](https://github.com/ddbeyin/keplemeyen-helper/releases), open it, and drag the application to your Applications folder.

## File Verification
Each release includes both a SHA256SUMS file and a detailed manifest.json for verifying file integrity. You can verify your download in two ways:

### Automatic Verification (Recommended)
1. Download the release package and extract it
2. Run `python verify.py` (Windows) or `./verify.sh` (Linux/macOS)
3. Check the verification results

### Manual Verification
For Windows:
```batch
certutil -hashfile KeplemeyenHelper.exe SHA256
```

For Linux/macOS:
```bash
shasum -a 256 -c SHA256SUMS
```

## Building from Source

### Windows
Prerequisites:
- Qt 6.5 or higher (MinGW version)
- CMake 3.19 or higher
- MinGW-w64 (compatible with your Qt version)
- Python 3.8 or higher

1. Clone the repository:
```bash
git clone https://github.com/ddbeyin/keplemeyen-helper.git
```

2. Set up your environment variables:
   - Set `Qt6_DIR` environment variable to point to your Qt's CMake directory:
     - Example: `C:\Qt\6.8.2\mingw_64\lib\cmake\Qt6`
   - Ensure MinGW is in your PATH environment variable:
     - Example: `C:\Qt\6.8.2\mingw_64\bin`

3. Run the build script:
```bash
build_and_hash.bat
```

### Linux/macOS
Prerequisites:
- Qt 6.5 or higher
- CMake 3.19 or higher
- GCC 11+ or Clang 13+
- Python 3.8 or higher

1. Clone the repository:
```bash
git clone https://github.com/ddbeyin/keplemeyen-helper.git
```

2. Build:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage
1. Launch the application
2. Load a log file (CSV or Excel format) either by:
   - Clicking the Load button and selecting a file
   - Dragging and dropping a file onto the application
3. Select an analysis module (currently Time Discrepancy available)
4. Click Analyze to process the file
5. Review results and use the generated response templates

## Development

### Setting Up Development Environment
1. Install Qt 6.5.0 or higher (MinGW version for Windows)
2. Install CMake 3.19 or higher
3. Set up environment variables:
   - `Qt6_DIR` pointing to your Qt CMake directory
   - MinGW in your PATH
4. Clone the repository
5. Use the provided build scripts or open the project in Qt Creator

### Build Scripts
The project includes several utility scripts:
- `build_and_hash.bat` - Builds the project and generates verification files
- `verify.bat` / `verify.sh` - Verifies file integrity
- `bump_version.bat` - Helps increment the version number

### Running Tests
```bash
cd build
ctest -V
```

### Generating Documentation
```bash
cd build
cmake --build . --target docs
```

## Troubleshooting

### Common Issues
1. **CMake can't find Qt**
   - Ensure `Qt6_DIR` environment variable is set correctly
   - It should point to the lib/cmake/Qt6 directory in your Qt installation
   - Example: `C:\Qt\6.8.2\mingw_64\lib\cmake\Qt6`

2. **CMake can't find a build program**
   - Ensure MinGW is properly installed
   - Add MinGW's bin directory to your PATH
   - Example: `C:\Qt\6.8.2\mingw_64\bin`

3. **Build fails**
   - Check CMake version is 3.19 or higher
   - Ensure all dependencies are installed
   - Clear build directory and try again

4. **Verification fails**
   - Download the release package again
   - Ensure all files were extracted
   - Check antivirus isn't blocking files

## Upcoming Features
- Collection and card database integration
- Additional analysis modules
- Enhanced response templates
- More game information reference tools

## Technical Details
Built with:
- C++ 17
- Qt 6.8.2 (licensed under LGPL v3 - see <https://www.gnu.org/licenses/lgpl-3.0.en.html>)
- CMake 3.19+
- MinGW-w64 compiler

## Contributing
Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute to this project.

## Security
For information about reporting security vulnerabilities, please see [SECURITY.md](SECURITY.md).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact
Developer: cdbrain
Email: ddbeyin@gmail.com
GitHub: [github.com/ddbeyin/keplemeyen-helper](https://github.com/ddbeyin/keplemeyen-helper)