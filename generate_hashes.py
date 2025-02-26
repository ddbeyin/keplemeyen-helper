#!/usr/bin/env python3
"""
Generate SHA256 hashes for build artifacts and create verification files.

This script generates:
1. SHA256SUMS file with hash and filename entries
2. manifest.json with detailed file information

Usage:
    python generate_hashes.py [directory]
"""

import os
import sys
import json
import hashlib
import datetime
from pathlib import Path

def calculate_sha256(file_path):
    """Calculate SHA256 hash of a file."""
    sha256_hash = hashlib.sha256()
    with open(file_path, "rb") as f:
        # Read the file in chunks to handle large files efficiently
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
    return sha256_hash.hexdigest()

def get_version(directory_path):
    """Get version from version.h."""
    version = "0.0.0"  # Default version
    
    # Try to find version.h in the build directory or parent directory
    version_paths = [
        directory_path / "version.h",
        directory_path.parent / "version.h"
    ]
    
    for version_file in version_paths:
        if version_file.exists():
            print(f"Reading version from: {version_file}")
            with open(version_file, "r") as f:
                for line in f:
                    if line.startswith('#define APP_VERSION_STR'):
                        try:
                            version = line.split('"')[1]
                            print(f"Found version: {version}")
                            return version
                        except:
                            print("Could not extract version, using default")
    
    print(f"Warning: Could not find or parse version.h, using default version: {version}")
    return version

def generate_manifest(directory):
    """Generate manifest.json and SHA256SUMS files."""
    directory_path = Path(directory)
    print(f"Searching for files in: {directory_path.absolute()}")
    
    # Get version information
    version = get_version(directory_path)
    
    # Find files to hash
    files_to_hash = []
    
    # Define extensions and patterns to look for
    extensions = ['.exe', '.dll', '.zip', '.deb', '.dmg', '.app']
    patterns = [
        "bin/**/*",
        "*.zip",
        "KeplemeyenHelper*"
    ]
    
    # Find by extension
    for ext in extensions:
        for file_path in directory_path.glob(f"**/*{ext}"):
            if file_path.is_file() and file_path not in files_to_hash:
                files_to_hash.append(file_path)
                print(f"Found file (by extension {ext}): {file_path.relative_to(directory_path)}")
    
    # Find by pattern
    for pattern in patterns:
        for file_path in directory_path.glob(pattern):
            if file_path.is_file() and file_path not in files_to_hash:
                files_to_hash.append(file_path)
                print(f"Found file (by pattern {pattern}): {file_path.relative_to(directory_path)}")
    
    # Delete existing SHA256SUMS and manifest.json if they exist
    for file in ["SHA256SUMS", "manifest.json"]:
        if (directory_path / file).exists():
            (directory_path / file).unlink()
            print(f"Deleted existing {file}")
    
    # Check if we found any files
    if not files_to_hash:
        print("Warning: No files found to hash!")
        print("Make sure you've built the project first.")
        
        # Create empty verification files anyway
        manifest = {
            "version": version,
            "generated": datetime.datetime.now().isoformat(),
            "files": [],
            "warning": "No files found to hash. Build the project first."
        }
        
        with open(directory_path / "manifest.json", "w") as f:
            json.dump(manifest, f, indent=2)
        
        with open(directory_path / "SHA256SUMS", "w") as f:
            f.write("# No files found to hash. Build the project first.\n")
        
        print(f"Generated empty manifest.json and SHA256SUMS in {directory}")
        return 0
    
    # Calculate hashes
    hash_entries = {}
    manifest_entries = []
    
    for file_path in files_to_hash:
        try:
            relative_path = file_path.relative_to(directory_path)
            file_hash = calculate_sha256(file_path)
            file_size = file_path.stat().st_size
            
            # Add to SHA256SUMS entries
            hash_entries[str(relative_path)] = file_hash
            
            # Add to manifest entries
            manifest_entries.append({
                "path": str(relative_path),
                "sha256": file_hash,
                "size": file_size
            })
            
            print(f"Hashed: {relative_path} ({file_size} bytes)")
        except Exception as e:
            print(f"Error processing {file_path}: {e}")
    
    # Create manifest.json
    manifest = {
        "version": version,
        "generated": datetime.datetime.now().isoformat(),
        "files": manifest_entries
    }
    
    # Write manifest.json
    with open(directory_path / "manifest.json", "w") as f:
        json.dump(manifest, f, indent=2)
    
    # Write SHA256SUMS
    with open(directory_path / "SHA256SUMS", "w") as f:
        for filepath, filehash in hash_entries.items():
            f.write(f"{filehash} *{filepath}\n")
    
    print(f"Generated manifest.json and SHA256SUMS in {directory}")
    return len(files_to_hash)

if __name__ == "__main__":
    directory = "." if len(sys.argv) < 2 else sys.argv[1]
    num_files = generate_manifest(directory)
    print(f"Processed {num_files} files")