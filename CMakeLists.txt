cmake_minimum_required(VERSION 3.12)

cmake_policy(SET CMP0042 NEW)
set(CMAKE_MACOSX_RPATH 0)

set(PROJECT sprint_timer_app)
project(${PROJECT} VERSION 0.4.8)

if (UNIX AND NOT APPLE)
    find_package(ECM REQUIRED NO_MODULE)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
    include(ECMEnableSanitizers)
endif()

option(BUILD_TESTS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/static)

include(compiler_options.cmake)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(DEBUG_COMPILE_OPTIONS ${CLANG_COMPILE_OPTIONS})
    set(RELEASE_COMPILE_OPTIONS ${CLANG_COMPILE_OPTIONS})
    if (ECM_ENABLE_SANITIZERS)
        message ("Enabled sanitizers: " "${ECM_ENABLE_SANITIZERS}")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(DEBUG_COMPILE_OPTIONS ${GCC_COMPILE_OPTIONS})
    set(RELEASE_COMPILE_OPTIONS ${GCC_COMPILE_OPTIONS})
elseif(MSVC)
	message("SprintTimer has experimental support for MSVC compiler")
	# Rely on default compiler options for now
else()
    message("Compiler might not be supported yet.")
endif()

set(GUI_LIB sprint_timer_qt_gui)
set(STORAGE_LIB sprint_timer_qt_storage_impl)
set(CORE_LIB sprint_timer_core)
set(SPRINT_MAIN sprint_timer)

set(EXECUTABLE_INSTALL_PATH_SUFFIX bin)
set(LIBRARY_INSTALL_PATH_SUFFIX lib)

add_subdirectory(core)
add_subdirectory(qt_storage_impl)
add_subdirectory(qt_gui)
add_subdirectory(main)

if (BUILD_TESTS)
    set(TEST_RUN_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif()
