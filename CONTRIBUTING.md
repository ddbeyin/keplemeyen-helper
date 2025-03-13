# Contributing to Keplemeyen Helper

Thank you for your interest in Keplemeyen Helper. Contributions from users help improve the tool for everyone.

## Code of Conduct

This project and everyone participating in it is governed by our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check the existing issues list as you might find out that you don't need to create one. When you are creating a bug report, please include as many details as possible:

* Use a clear and descriptive title
* Describe the exact steps which reproduce the problem
* Provide specific examples to demonstrate the steps
* Describe the behavior you observed after following the steps
* Explain which behavior you expected to see instead and why
* Include screenshots if possible
* Include your environment details (OS, Qt version, etc.)

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, please include:

* Use a clear and descriptive title
* Provide a step-by-step description of the suggested enhancement
* Provide specific examples to demonstrate the steps
* Describe the current behavior and explain which behavior you expected to see instead
* Explain why this enhancement would be useful

### Pull Requests

* Fill in the required template
* Do not include issue numbers in the PR title
* Follow the C++ coding style
* Include appropriate test cases
* Document new code with clear comments
* End all files with a newline

## Development Process

1. Fork the repo
2. Create a new branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Commit your changes (`git commit -m 'Add some amazing feature'`)
5. Push to the branch (`git push origin feature/amazing-feature`)
6. Open a Pull Request

## Development Environment Setup

### Prerequisites
- Qt 6.5 or higher (6.8.2 recommended)
- CMake 3.19 or higher
- C++17 compatible compiler

### Windows Setup
1. **Install Qt**
   - Download from [Qt's official website](https://www.qt.io/download) (open source version)
   - Select Qt 6.5 or higher with MinGW components
   - The default installation path is typically `C:\Qt`

2. **Install CMake** (if not using Qt's bundled version)
   - Download from [CMake's official website](https://cmake.org/download/)
   - Add CMake to your PATH during installation
   - Alternatively, use Qt's bundled CMake located at `C:\Qt\Tools\CMake_64\bin`

3. **Set Environment Variables**
   - Add MinGW to your PATH:
     - Add `C:\Qt\Tools\mingw1310_64\bin` to your PATH environment variable (adjust based on your Qt installation)
   - Add Qt bin directory to your PATH:
     - Add `C:\Qt\6.8.2\mingw_64\bin` to your PATH environment variable (adjust to match your Qt version)

4. **Verify Setup**
   - Open a new command prompt (important for environment variables to take effect)
   - Run `g++ --version` to check MinGW
   - Run `cmake --version` to check CMake
   - Run `qmake --version` to check Qt

### Linux Setup
1. Install Qt: `sudo apt install qt6-base-dev` or similar for your distribution
2. Install CMake: `sudo apt install cmake`
3. Install build tools: `sudo apt install build-essential`

### macOS Setup
1. Install [Homebrew](https://brew.sh/)
2. Install Qt: `brew install qt@6`
3. Install CMake: `brew install cmake`

## Build Instructions

### Using the Build Script (Windows)
1. Edit the `build_installer.bat` script to match your Qt and MinGW installation paths:
   ```batch
   set "QT_DIR=C:\Qt\6.8.2\mingw_64"
   set "MINGW_DIR=C:\Qt\Tools\mingw1310_64\bin"
   set "CMAKE_DIR=C:\Qt\Tools\CMake_64\bin"
   ```

2. Run the script:
   ```batch
   build_installer.bat
   ```

### Manual Build (All Platforms)
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

## Versioning Process

When you're ready to release a new version:

1. Update version.h with the new version number
   
2. Update CHANGELOG.md with the changes
   
3. Commit the changes and create a tag:
   ```bash
   git add version.h CHANGELOG.md
   git commit -m "Bump version to x.y.z"
   git tag vx.y.z
   git push origin main --tags
   ```

4. Create a new release on GitHub:
   - Go to the GitHub repository page
   - Navigate to "Releases" > "Create a new release"
   - Select the tag you just pushed
   - Fill in the release title and description
   - Upload the manually created installer (`KeplemeyenHelper-x.y.z-win64.exe`)
   - Click "Publish release"

## Styleguides

### Git Commit Messages

* Use the present tense ("Add feature" not "Added feature")
* Use the imperative mood ("Move cursor to..." not "Moves cursor to...")
* Limit the first line to 72 characters or less
* Reference issues and pull requests liberally after the first line

### C++ Styleguide

* Follow the Qt coding style guidelines
* Use modern C++ features (C++17) when appropriate
* Use smart pointers instead of raw pointers where possible
* Document all public methods with clear comments
* Keep functions focused and short
* Use meaningful variable names
* Include appropriate error handling

### Documentation Styleguide

* Use clear, concise comments for code documentation
* Write documentation in clear, concise English
* Include examples where appropriate
* Keep line length reasonable
* Document both the what and the why

## Testing

* Write unit tests for all new functionality
* Run existing tests before submitting your changes: `ctest -V` from the build directory
* Ensure all tests pass on all supported platforms

## Additional Notes

### Issue and Pull Request Labels

| Label Name | Description |
| --- | --- |
| `bug` | Confirmed bugs or reports that are very likely to be bugs |
| `enhancement` | Feature requests |
| `documentation` | Documentation improvements |
| `good first issue` | Good for newcomers |
| `help wanted` | Extra attention is needed |

## Release Process

1. Update version.h with the new version number
2. Update CHANGELOG.md with the changes
3. Create a new tag following semantic versioning:
   ```bash
   git add version.h CHANGELOG.md
   git commit -m "Bump version to x.y.z"
   git tag vx.y.z
   git push origin main --tags
   ```
4. Build the installer manually using the build_installer.bat script
5. Create a new release on GitHub and upload the installer:
   - Go to the GitHub repository page
   - Navigate to "Releases" > "Create a new release"
   - Select the tag you just pushed
   - Fill in the release title and description
   - Upload the manually built installer (e.g., `KeplemeyenHelper-0.2.0-win64.exe`)
   - Click "Publish release"

Thank you for contributing!