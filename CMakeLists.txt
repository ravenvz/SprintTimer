cmake_minimum_required(VERSION 3.15)

if (POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
if (POLICY CMP0076)
    cmake_policy(SET CMP0076 NEW)
endif()

project(sprint_timer_app VERSION 0.6.5)

# Download CPM
include(cmake/CpmDownload.cmake)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_MACOSX_RPATH 0)

# names of binaries that are used to link against
set(GUI_LIB sprint_timer_qt_gui)
set(STORAGE_LIB sprint_timer_qt_storage)
set(CORE_LIB sprint_timer_core)
set(SPRINT_MAIN sprint_timer)
set(EXTERNAL_IO_LIB external_io)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(sprint_timer_options INTERFACE)
target_compile_features(sprint_timer_options INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        target_compile_options(sprint_timer_options INTERFACE -ftime-trace)
    endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(sprint_timer_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_sprint_timer_warnings(sprint_timer_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(sprint_timer_options)

# enable doxygen
# include(cmake/Doxygen.cmake)
# enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" OFF)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

add_subdirectory(src)

if(ENABLE_TESTING)
    set(TEST_RUN_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    enable_testing()
	include(GoogleTest)
    add_subdirectory(tests)
endif()

# Package builder
set(CPACK_PACKAGE_CONTACT "Pavel Pavlov <texort at gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Pavel Pavlov")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT}")

if (APPLE)
    set(CPACK_BUNDLE_NAME "SprintTimer")
    set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_LIST_DIR}/mac_os/Info.plist")
    set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_LIST_DIR}/mac_os/iconbuilder.icns")
    set(CPACK_BUNDLE_STARTUP_COMMAND "${CMAKE_CURRENT_LIST_DIR}/mac_os/startup.sh")
    set(CPACK_GENERATOR "Bundle")
    set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/mac_os/image_creating.scpt")
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_LIST_DIR}/mac_os/background.png")
    set(CPACK_DMG_FORMAT "UDBZ")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    set(CPACK_SYSTEM_NAME "OSX")
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/qt_gui/resources/icons/sprint_timer.png")
endif()

include(CPack)
