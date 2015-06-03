cmake_minimum_required(VERSION 3.2)

project(test_pomodoro)
set(SOURCE_FILES
    run_all_tests.cpp
    TestTaskScheduler.cpp
    ../src/core/TaskScheduler.cpp
)
find_library(CppUTest REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable(test_pomodoro ${SOURCE_FILES})

include_directories("/usr/include/CppUTest")
qt5_use_modules(test_pomodoro Core)
target_link_libraries(test_pomodoro CppUTest)
