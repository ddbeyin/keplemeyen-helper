#!/usr/bin/env python3
"""
Simple file verification script that checks SHA256 hashes.
"""

import os
import sys
import hashlib
from pathlib import Path

def calculate_sha256(file_path):
    """Calculate SHA256 hash of a file."""
    sha256_hash = hashlib.sha256()
    with open(file_path, "rb") as f:
        # Read the file in chunks to handle large files efficiently
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

def verify_files():
    """Verify files against SHA256SUMS."""
    # Determine which directory to check
    if os.path.exists(os.path.join("build", "SHA256SUMS")):
        check_dir = "build"
        print("Found SHA256SUMS in build directory, verifying there...")
    elif os.path.exists("SHA256SUMS"):
        check_dir = "."
        print("Found SHA256SUMS in current directory...")
    else:
        print("ERROR: SHA256SUMS file not found")
        print("Please run this script in the directory containing SHA256SUMS")
        print("or in the parent directory of the build folder.")
        return 1
    
    # Change to the check directory
    os.chdir(check_dir)
    
    # Read the SHA256SUMS file
    print("Verifying file integrity...\n")
    
    total_files = 0
    verified_files = 0
    failed_files = 0
    
    with open("SHA256SUMS", "r") as f:
        for line in f:
            parts = line.strip().split(" ", 1)
            if len(parts) != 2:
                continue
                
            expected_hash = parts[0]
            file_path = parts[1]
            
            # Remove leading asterisk if present
            if file_path.startswith("*"):
                file_path = file_path[1:]
            
            total_files += 1
            print(f"Verifying: {file_path}")
            
            if os.path.exists(file_path):
                try:
                    actual_hash = calculate_sha256(file_path)
                    
                    if actual_hash.lower() == expected_hash.lower():
                        print("  Status: VERIFIED [OK]")
                        verified_files += 1
                    else:
                        print("  Status: FAILED - Hash mismatch")
                        print(f"  Expected: {expected_hash}")
                        print(f"  Actual:   {actual_hash}")
                        failed_files += 1
                except Exception as e:
                    print(f"  Status: FAILED - Error: {e}")
                    failed_files += 1
            else:
                print("  Status: FAILED - File not found")
                failed_files += 1
            
            print()
    
    print("Verification complete:")
    print(f"  Total files: {total_files}")
    print(f"  Verified files: {verified_files}")
    print(f"  Failed files: {failed_files}")
    
    # Return to original directory if needed
    if check_dir != ".":
        os.chdir("..")
    
    if failed_files > 0:
        print("\nWARNING: Some files failed verification")
        return 1
    else:
        print("\nSUCCESS: All files verified successfully")
        return 0

if __name__ == "__main__":
    sys.exit(verify_files())