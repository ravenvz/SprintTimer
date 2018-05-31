cmake_minimum_required(VERSION 3.11)

set(PROJECT sprint_timer)
project(${PROJECT})

find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Multimedia)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#find_package(ECM REQUIRED NO_MODULE)
#set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
#include(ECMEnableSanitizers)

option(BUILD_TESTS ON)

#if (ECM_ENABLE_SANITIZERS)
    # Sanitizers do not play well with CppUTest memory leak detection plugin
#    set(BUILD_TESTS OFF)
#endif()

set("${PROJECT}_BINARY_DIR" bin)
set("${PROJECT}_SOURCE_DIR" src)
set("${PROJECT}_LIB_DIR" lib)
set("${PROJECT}_ARCHIVE_DIR" lib/static)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${${PROJECT}_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${${PROJECT}_ARCHIVE_DIR})

include(compiler_options.cmake)

set(SOURCE_FILES src/main.cpp)

add_executable(${PROJECT} ${SOURCE_FILES})
target_compile_features(${PROJECT} PUBLIC cxx_std_17)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(DEBUG_COMPILE_OPTIONS ${CLANG_COMPILE_OPTIONS})
    set(RELEASE_COMPILE_OPTIONS ${CLANG_COMPILE_OPTIONS})
    target_compile_options(${PROJECT} PUBLIC ${CLANG_COMPILE_OPTIONS})
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(DEBUG_COMPILE_OPTIONS ${GCC_COMPILE_OPTIONS})
    set(RELEASE_COMPILE_OPTIONS ${GCC_COMPILE_OPTIONS})
    target_compile_options(${PROJECT} PUBLIC ${GCC_COMPILE_OPTIONS})
else()
    message("Compiler might not be supported yet.")
endif()

target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

set(GUI_LIB sprint_timer_qt_gui)
set(STORAGE_LIB sprint_timer_qt_storage_impl)
set(CORE_LIB sprint_timer_core)

target_include_directories(${PROJECT} PUBLIC src lib)
target_link_libraries(${PROJECT}
    PRIVATE ${GUI_LIB}
    PRIVATE ${STORAGE_LIB}
)

# TODO remove when CMake 3.10 is supported by MSCV
# Workaround until CMake would be able to pass cxx_std_17 to MSCV correctly
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++17" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++17")
    endif()
endif()

set (EXECUTABLE_INSTALL_PATH_SUFFIX bin)
set (LIBRARY_INSTALL_PATH_SUFFIX lib)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})

add_subdirectory(core)
add_subdirectory(qt_gui)
add_subdirectory(qt_storage_impl)

if (BUILD_TESTS)
    enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif()
