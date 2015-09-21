cmake_minimum_required(VERSION 3.2)

project(test_pomodoro)
set(SOURCE_FILES
    run_all_tests.cpp
    TestTaskScheduler.cpp
    test_pomodoro_gateway.cpp
    test_todoitem.cpp
    ../src/core/TaskScheduler.cpp
        ../src/core/entities/TodoItem.cpp
        ../src/db_layer/db_helper.cpp
        ../src/core/entities/TodoItem.h
    ../src/core/config.h
)
find_library(CppUTest REQUIRED)
find_package(Qt5 REQUIRED Core Sql)

add_executable(test_pomodoro ${SOURCE_FILES})

include_directories("/usr/include/CppUTest")
qt5_use_modules(test_pomodoro Core Sql)
target_link_libraries(test_pomodoro CppUTest)
