cmake_minimum_required(VERSION 3.11)

set(PROJECT sprint_timer)
project(${PROJECT})

#find_package(ECM REQUIRED NO_MODULE)
#set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
#include(ECMEnableSanitizers)

option(BUILD_TESTS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/static)

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
elseif(MSVC)
	message("SprintTimer has experimental support for MSVC compiler")
	# Rely on default compiler options for now
else()
    message("Compiler might not be supported yet.")
endif()

target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

set(GUI_LIB sprint_timer_qt_gui)
set(STORAGE_LIB sprint_timer_qt_storage_impl)
set(CORE_LIB sprint_timer_core)

target_link_libraries(${PROJECT}
    PRIVATE ${GUI_LIB}
    PRIVATE ${STORAGE_LIB}
)

set (EXECUTABLE_INSTALL_PATH_SUFFIX bin)
set (LIBRARY_INSTALL_PATH_SUFFIX lib)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_PATH_SUFFIX})

# Download date_wrapper lib at configure time then add as subdirectory
include(CMakeLists-datewrapper.txt)

add_subdirectory(core)
add_subdirectory(qt_gui)
add_subdirectory(qt_storage_impl)

if (BUILD_TESTS)
    enable_testing()
	include(GoogleTest)
	add_subdirectory(tests)
endif()
