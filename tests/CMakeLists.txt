cmake_minimum_required(VERSION 3.2)

set(TEST_PROJECT acceptance_tests_stub)
project(${TEST_PROJECT})
set(SOURCE_FILES
    run_all_tests.cpp
)

find_package(Qt5 REQUIRED Core Sql Widgets)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TEST_PROJECT} ${SOURCE_FILES})
target_compile_features(${TEST_PROJECT} PRIVATE cxx_return_type_deduction)

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${TEST_PROJECT} PRIVATE ../src)

target_link_libraries(${TEST_PROJECT} CppUTest Qt5::Core Qt5::Sql Qt5::Widgets Threads::Threads)

# This is a workaround 
# For some reason, although CppUTest explicitly depends on winmm, dependency does not transition.
if (WIN32)
    target_link_libraries(${TEST_PROJECT} winmm.lib)
endif()
