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
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Configure version header
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    @ONLY
)

# Find Qt6
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# Set application icon
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/src/app_icon.rc")

# Add executable
qt_add_executable(KeplemeyenHelper
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/csvparser.cpp
    src/csvparser.h
    src/resources.qrc
    ${APP_ICON_RESOURCE_WINDOWS}
)

target_include_directories(KeplemeyenHelper PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(KeplemeyenHelper
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# Installation settings
include(GNUInstallDirs)

# Windows deployment
if(WIN32)
    # Find windeployqt executable
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    # Create a deployment directory
    set(DEPLOY_DIR "${CMAKE_BINARY_DIR}/deploy")
    file(MAKE_DIRECTORY ${DEPLOY_DIR})

    # Install the executable
    install(TARGETS KeplemeyenHelper RUNTIME DESTINATION "bin")
    
    # Deploy Qt dependencies using windeployqt
    install(CODE "
        set(QT_DEPLOY_FLAGS --verbose 0 --no-translations --no-system-d3d-compiler --no-opengl-sw)
        execute_process(
            COMMAND \"${WINDEPLOYQT_EXECUTABLE}\" \${QT_DEPLOY_FLAGS} --dir \"\${CMAKE_INSTALL_PREFIX}/bin\" \"\${CMAKE_INSTALL_PREFIX}/bin/KeplemeyenHelper.exe\"
            RESULT_VARIABLE RESULT
        )
        if(NOT RESULT EQUAL 0)
            message(FATAL_ERROR \"windeployqt failed: \${RESULT}\")
        endif()
    ")
endif()

# CPack configuration
set(CPACK_PACKAGE_NAME "KeplemeyenHelper")
set(CPACK_PACKAGE_VENDOR "ddbeyin")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A comprehensive toolkit for game support agents")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "KeplemeyenHelper")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Installation registry key
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "KeplemeyenHelper${PROJECT_VERSION}")

# Windows installer settings
if(WIN32)
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/KeplemeyenHelper.exe")
    set(CPACK_NSIS_DISPLAY_NAME "Keplemeyen Helper ${PROJECT_VERSION}")
    set(CPACK_NSIS_PACKAGE_NAME "Keplemeyen Helper")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/src/app_icon.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/src/app_icon.ico")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_MODIFY_PATH OFF)
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    set(CPACK_NSIS_HELP_LINK "https://github.com/ddbeyin/keplemeyen-helper")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/ddbeyin/keplemeyen-helper")
    set(CPACK_NSIS_CONTACT "ddbeyin@gmail.com")
    
    # Disable finish page run option
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN OFF)
    
    # Create Start Menu shortcuts
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Keplemeyen Helper.lnk' '$INSTDIR\\\\bin\\\\KeplemeyenHelper.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Keplemeyen Helper.lnk'"
    )
    
    # Add desktop shortcut
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        CreateShortCut '$DESKTOP\\\\Keplemeyen Helper.lnk' '$INSTDIR\\\\bin\\\\KeplemeyenHelper.exe'
    ")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
        Delete '$DESKTOP\\\\Keplemeyen Helper.lnk'
    ")
endif()

# Include CPack
include(CPack)