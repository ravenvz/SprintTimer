cmake_minimum_required(VERSION 3.2)

project(test_pomodoro)
set(SOURCE_FILES
    run_all_tests.cpp
    test_task_scheduler.cpp
    test_math_utils.cpp
    test_pomodoro_model.cpp
    test_pomodoro_statistics.cpp
    test_todoitem.cpp
    test_todoitem_model.cpp
    ../src/core/PomodoroStatistics.cpp
    ../src/core/TaskScheduler.cpp
    ../src/core/config.h
    ../src/core/entities/Pomodoro.cpp
    ../src/core/entities/TodoItem.cpp
    ../src/core/entities/TodoItem.h
    ../src/db_layer/db_helper.cpp
        ../src/models/todoitemmodel.cpp
        ../src/models/pomodoromodel.cpp
    ../src/utils/MathUtils.cpp
)
# find_library(CppUTest REQUIRED)
find_package(Qt5 REQUIRED Core Sql)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # silence warnings from CppUTest
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-macro -Wno-inconsistent-missing-override") 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro -Wno-disabled-macro-expansion") 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-old-style-cast") 
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -march=native -mtune=native -DNDEBUG")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -pedantic-errors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -march=native -mtune=native -DNDEBUG")
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

add_executable(test_pomodoro ${SOURCE_FILES})

include_directories("/usr/include/CppUTest")
# include_directories("../libs/cpputest/include/CppUTest/*")
qt5_use_modules(test_pomodoro Core Sql)
target_link_libraries(test_pomodoro CppUTest)
