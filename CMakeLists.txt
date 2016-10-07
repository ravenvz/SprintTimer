cmake_minimum_required(VERSION 3.6)

set(PROJECT sprint_timer)
project(${PROJECT})

find_package(Qt5 REQUIRED COMPONENTS Core Sql Widgets Multimedia)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
include(ECMEnableSanitizers)

option(BUILD_TESTS ON)

if (ECM_ENABLE_SANITIZERS)
    # Sanitizers do not play well with CppUTest memory leak detection plugin
    set(BUILD_TESTS OFF)
endif()

set("${PROJECT}_BINARY_DIR" bin)
set("${PROJECT}_SOURCE_DIR" src)
set("${PROJECT}_LIB_DIR" lib)
set("${PROJECT}_ARCHIVE_DIR" lib/static)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_BINARY_DIR})
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_LIB_DIR})
set (ARCHIVE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/${${PROJECT}_ARCHIVE_DIR})
set (CMAKE_VERBOSE_MAKEFILE OFF)

set (ERR_NO_UNIX "Cannot build on non Unix systems")

if (WITH_DEBUG_MODE)
    ADD_DEFINITIONS( -DMY_DEBUG_MODE=1)
endif()

find_program(MEMORYCHECK_COMMAND valgrind)
if(MEMORYCHECK_COMMAND-NOTFOUND)
  message("Valgrind not found")
else()
  message("Valgrind: ${MEMORYCHECK_COMMAND}")
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
endif()

include(compiler_options.cmake)

set(SOURCE_FILES src/main.cpp)

add_executable(${PROJECT} ${SOURCE_FILES})
target_compile_features(${PROJECT} PUBLIC cxx_return_type_deduction)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${PROJECT} PUBLIC ${CLANG_COMPILE_OPTIONS})
elseif(CMAKE_COMPILER_IS_GNUCXX)
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
    PRIVATE stdc++fs
)

set (EXECUTABLE_INSTALL_PATH_SUFFIX bin)
set (LIBRARY_INSTALL_PATH_SUFFIX lib)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})

add_subdirectory(core)
add_subdirectory(qt_gui)
add_subdirectory(qt_storage_impl)

if (BUILD_TESTS)
    enable_testing() # TODO Ctest is not yet configured
    add_subdirectory(tests)
endif()
