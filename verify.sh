#!/bin/bash

echo "===== Keplemeyen Helper Verification Script ====="

# Determine directory to check
CHECK_DIR="."
if [ -f "build/SHA256SUMS" ]; then
    CHECK_DIR="build"
    echo "Found SHA256SUMS in build directory, verifying there..."
elif [ ! -f "SHA256SUMS" ]; then
    echo "ERROR: SHA256SUMS file not found"
    echo "Please run this script in the directory containing SHA256SUMS"
    echo "or in the parent directory of the build folder."
    exit 1
fi

cd "$CHECK_DIR"

# Check for shasum or sha256sum
HASH_CMD=""
if command -v shasum &> /dev/null; then
    HASH_CMD="shasum -a 256 -c"
    echo "Using shasum for verification"
elif command -v sha256sum &> /dev/null; then
    HASH_CMD="sha256sum -c"
    echo "Using sha256sum for verification"
else
    echo "ERROR: Neither shasum nor sha256sum found"
    echo "Please install the required tools:"
    echo "  - On macOS: brew install coreutils"
    echo "  - On Linux: apt-get install coreutils (or equivalent)"
    if [ "$CHECK_DIR" != "." ]; then
        cd ..
    fi
    exit 1
fi

echo "Verifying file integrity..."
echo

# Run the appropriate verification command
$HASH_CMD SHA256SUMS
VERIFY_RESULT=$?

if [ $VERIFY_RESULT -eq 0 ]; then
    echo
    echo "SUCCESS: All files verified successfully"
    if [ "$CHECK_DIR" != "." ]; then
        cd ..
    fi
    exit 0
else
    echo
    echo "WARNING: Some files failed verification"
    if [ "$CHECK_DIR" != "." ]; then
        cd ..
    fi
    exit 1
fi