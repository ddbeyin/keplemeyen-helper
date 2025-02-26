cmake_minimum_required(VERSION 3.19)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.h" VERSION_CONTENT)

string(REGEX MATCH "APP_VERSION_MAJOR ([0-9]+)" _ ${VERSION_CONTENT})
set(VERSION_MAJOR ${CMAKE_MATCH_1})

string(REGEX MATCH "APP_VERSION_MINOR ([0-9]+)" _ ${VERSION_CONTENT})
set(VERSION_MINOR ${CMAKE_MATCH_1})

string(REGEX MATCH "APP_VERSION_PATCH ([0-9]+)" _ ${VERSION_CONTENT})
set(VERSION_PATCH ${CMAKE_MATCH_1})

project(KeplemeyenHelper
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    LANGUAGES CXX
    DESCRIPTION "A comprehensive toolkit for game support agents"
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Make Doxygen optional for CI builds
option(DOXYGEN_REQUIRED "Require Doxygen for documentation" ON)

# Add Doxygen support
find_package(Doxygen
    ${DOXYGEN_REQUIRED} # REQUIRED becomes conditional
    OPTIONAL_COMPONENTS mscgen dia
)

set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR})
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
)

# Add documentation targets if Doxygen was found
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )

    # Install documentation
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            OPTIONAL)
else()
    message(STATUS "Doxygen not found - documentation will not be built")
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    @ONLY
)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(KeplemeyenHelper
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    csvparser.cpp
    csvparser.h
    resources.qrc
)

target_include_directories(KeplemeyenHelper PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(KeplemeyenHelper
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS KeplemeyenHelper
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install documentation if available
if(DOXYGEN_FOUND)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            OPTIONAL)
endif()

qt_generate_deploy_app_script(
    TARGET KeplemeyenHelper
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# CPack configuration
include(CPack)
set(CPACK_PACKAGE_NAME "KeplemeyenHelper")
set(CPACK_PACKAGE_VENDOR "ddbeyin")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A comprehensive toolkit for game support agents")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_PACKAGE_NAME "Keplemeyen Helper")
    set(CPACK_NSIS_DISPLAY_NAME "Keplemeyen Helper")
    set(CPACK_NSIS_HELP_LINK "https://github.com/ddbeyin/keplemeyen-helper")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/ddbeyin/keplemeyen-helper")
    set(CPACK_NSIS_CONTACT "ddbeyin@gmail.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "Keplemeyen Helper")
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ddbeyin@gmail.com")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6 (>= 6.5.0), libqt6widgets6 (>= 6.5.0)")
endif()