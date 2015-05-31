cmake_minimum_required(VERSION 3.2)

project(test_pomodoro)
set(SOURCE_FILES
    run_all_tests.cpp
    TestTaskScheduler.cpp
    ../src/core/TaskScheduler.cpp
)
find_library(CppUTest REQUIRED)

add_executable(test_pomodoro ${SOURCE_FILES})

include_directories("/usr/include/CppUTest")
target_link_libraries(test_pomodoro CppUTest)
