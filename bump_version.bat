@echo off
setlocal enabledelayedexpansion

echo ===== Keplemeyen Helper Version Bumper =====

if not exist version.h (
    echo ERROR: version.h not found
    exit /b 1
)

REM Read current version
for /f "tokens=3" %%i in ('findstr /C:"APP_VERSION_MAJOR" version.h') do set MAJOR=%%i
for /f "tokens=3" %%i in ('findstr /C:"APP_VERSION_MINOR" version.h') do set MINOR=%%i
for /f "tokens=3" %%i in ('findstr /C:"APP_VERSION_PATCH" version.h') do set PATCH=%%i

echo Current version: %MAJOR%.%MINOR%.%PATCH%
echo.
echo Which version component do you want to bump?
echo 1. Major (x.0.0)
echo 2. Minor (0.x.0)
echo 3. Patch (0.0.x)
echo.

set /p CHOICE="Enter choice (1-3): "

if "%CHOICE%"=="1" (
    set /a MAJOR=%MAJOR%+1
    set MINOR=0
    set PATCH=0
    set VERSION_TYPE=Major
) else if "%CHOICE%"=="2" (
    set /a MINOR=%MINOR%+1
    set PATCH=0
    set VERSION_TYPE=Minor
) else if "%CHOICE%"=="3" (
    set /a PATCH=%PATCH%+1
    set VERSION_TYPE=Patch
) else (
    echo Invalid choice
    exit /b 1
)

set NEW_VERSION=%MAJOR%.%MINOR%.%PATCH%
echo New version will be: %NEW_VERSION%
set /p CONFIRM="Continue? (y/n): "

if /i not "%CONFIRM%"=="y" (
    echo Operation cancelled
    exit /b 0
)

REM Update version.h
echo Updating version.h...
(
    for /f "tokens=1,* delims=:" %%a in ('findstr /n "^" version.h') do (
        set "line=%%b"
        if "!line!" == "" (
            rem Empty line
            echo.
        ) else if "!line:~0,20!" == "#define APP_VERSION_" (
            if "!line:~20,5!" == "MAJOR" (
                echo #define APP_VERSION_MAJOR %MAJOR%
            ) else if "!line:~20,5!" == "MINOR" (
                echo #define APP_VERSION_MINOR %MINOR%
            ) else if "!line:~20,5!" == "PATCH" (
                echo #define APP_VERSION_PATCH %PATCH%
            ) else if "!line:~20,3!" == "STR" (
                echo #define APP_VERSION_STR "%NEW_VERSION%"
            ) else (
                echo !line!
            )
        ) else (
            echo !line!
        )
    )
) > version.h.new
move /y version.h.new version.h

REM Update CHANGELOG.md if it exists
if exist CHANGELOG.md (
    echo Updating CHANGELOG.md...

    REM Get current date in YYYY-MM-DD format
    for /f "tokens=2 delims==" %%G in ('wmic os get localdatetime /value') do set datetime=%%G
    set CURRENT_DATE=%datetime:~0,4%-%datetime:~4,2%-%datetime:~6,2%
    
    echo Enter changelog entry for version %NEW_VERSION%:
    echo (Type your changes, press Enter after each line, and press Ctrl+Z followed by Enter when done)
    echo.
    
    REM Create a temporary file to store the changelog entry
    type nul > temp_changelog_entry.txt
    copy con temp_changelog_entry.txt > nul
    
    echo Creating new CHANGELOG.md...
    (
        echo # Changelog
        echo All notable changes to this project will be documented in this file.
        echo.
        echo The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/^),
        echo and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html^).
        echo.
        echo ## [%NEW_VERSION%] - %CURRENT_DATE%
        echo ### %VERSION_TYPE%
        type temp_changelog_entry.txt
        echo.
        
        REM Skip the first 5 lines of the original file and add the rest
        for /f "skip=5 tokens=1,* delims=:" %%a in ('findstr /n "^" CHANGELOG.md') do (
            set "line=%%b"
            if "!line!" == "" (
                echo.
            ) else (
                echo !line!
            )
        )
    ) > CHANGELOG.md.new
    
    move /y CHANGELOG.md.new CHANGELOG.md
    del temp_changelog_entry.txt
)

echo.
echo Version bumped successfully to %NEW_VERSION%
echo.
echo Don't forget to:
echo 1. Commit the changes with:
echo    git add version.h CHANGELOG.md
echo    git commit -m "Bump version to %NEW_VERSION%"
echo 2. Create a tag:
echo    git tag v%NEW_VERSION%
echo 3. Push the changes and tag:
echo    git push origin main --tags

exit /b 0