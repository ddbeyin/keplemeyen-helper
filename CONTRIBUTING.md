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
* Document new code based on the Documentation Styleguide
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
- Qt 6.5 or higher (MinGW version for Windows)
- CMake 3.19 or higher
- C++17 compatible compiler
- Python 3.8 or higher (for build scripts)

### Windows Setup
1. **Install Qt**
   - Download from [Qt's official website](https://www.qt.io/download) (open source version)
   - Select Qt 6.5 or higher with MinGW components
   - The default installation path is typically `C:\Qt`

2. **Install CMake**
   - Download from [CMake's official website](https://cmake.org/download/)
   - Add CMake to your PATH during installation

3. **Set Environment Variables**
   - Set `Qt6_DIR` environment variable to point to your Qt's CMake directory:
     - Navigate to System Properties > Advanced > Environment Variables
     - Add a new System variable:
       - Name: `Qt6_DIR`
       - Value: `C:\Qt\6.8.2\mingw_64\lib\cmake\Qt6` (adjust to match your Qt version)
   - Ensure MinGW is in your PATH:
     - Add `C:\Qt\6.8.2\mingw_64\bin` to your PATH environment variable (adjust to match your Qt version)

4. **Verify Setup**
   - Open a new command prompt (important for environment variables to take effect)
   - Run `g++ --version` to check MinGW
   - Run `cmake --version` to check CMake

### Linux Setup
1. Install Qt: `sudo apt install qt6-base-dev` or similar for your distribution
2. Install CMake: `sudo apt install cmake`
3. Install build tools: `sudo apt install build-essential`

### macOS Setup
1. Install [Homebrew](https://brew.sh/)
2. Install Qt: `brew install qt@6`
3. Install CMake: `brew install cmake`

## Build Instructions

### Using Build Scripts (Windows)
1. Clone the repository:
   ```bash
   git clone https://github.com/ddbeyin/keplemeyen-helper.git
   cd keplemeyen-helper
   ```

2. Run the build script:
   ```bash
   build_and_hash.bat
   ```

3. Verify the build:
   ```bash
   verify.bat
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
     cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
     ```
   - Linux/macOS:
     ```bash
     cmake .. -DCMAKE_BUILD_TYPE=Release
     ```

3. Build the project:
   ```bash
   cmake --build .
   ```

## Versioning Process

When you're ready to release a new version:

1. Use the version bumping script:
   ```bash
   bump_version.bat
   ```
   
2. Follow the prompts to increment the major, minor, or patch version
   
3. Commit the changes and create a tag:
   ```bash
   git add version.h CHANGELOG.md
   git commit -m "Bump version to x.y.z"
   git tag vx.y.z
   git push origin main --tags
   ```

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
* Document all public methods using Doxygen-style comments
* Keep functions focused and short
* Use meaningful variable names
* Include appropriate error handling

### Documentation Styleguide

* Use Doxygen for code documentation
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

1. Update version.h with the new version number (use bump_version.bat)
2. Update CHANGELOG.md with the changes
3. Create a new tag following semantic versioning
4. Push the tag to trigger the release workflow
5. Verify the release artifacts

Thank you for contributing!