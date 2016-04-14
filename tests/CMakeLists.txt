cmake_minimum_required(VERSION 3.2)

set(TEST_PROJECT test_pomodoro)
project(${TEST_PROJECT})
set(SOURCE_FILES
    run_all_tests.cpp
    test_math_utils.cpp
    test_pomodoro_model.cpp
    test_pomodoro_statistics.cpp
    test_todoitem.cpp
    test_todoitem_model.cpp
    test_datetime.cpp
    test_time_span.cpp
    test_pomodoro_timer.cpp
    ../src/core/PomodoroStatistics.cpp
    ../src/core/PomodoroTimer.h
    ../src/core/PomodoroTimer.cpp
    ../src/core/Timer.h
    ../src/core/Timer.cpp
    ../src/core/config.h
    ../src/core/entities/Pomodoro.cpp
    ../src/core/entities/TodoItem.cpp
    ../src/core/entities/TodoItem.h
    ../src/core/TimeSpan.h
    ../src/core/TimeSpan.cpp
    ../src/core/DateTime.h
    ../src/core/DateTime.cpp
    ../src/db_layer/db_service.cpp
    ../src/models/todoitemmodel.cpp
    ../src/models/pomodoromodel.cpp
    ../src/utils/MathUtils.cpp
    ../src/utils/StringUtils.h
    ../src/utils/StringUtils.cpp
)
# find_library(CppUTest REQUIRED)
find_package(Qt5 REQUIRED Core Sql)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TEST_PROJECT} ${SOURCE_FILES})
target_compile_features(${TEST_PROJECT} PRIVATE cxx_return_type_deduction)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
#   # silence warnings from CppUTest
    target_compile_options(${TEST_PROJECT} PRIVATE ${CLANG_COMPILE_OPTIONS}
                                                     -Wno-keyword-macro
                                                     -Wno-inconsistent-missing-override
                                                     -Wno-reserved-id-macro
                                                     -Wno-disabled-macro-expansion
                                                     -Wno-old-style-cast)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TEST_PROJECT} PRIVATE ${GCC_COMPILE_OPTIONS}
                                                   -Wno-unused-parameter)
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${TEST_PROJECT} PRIVATE /usr/include/CppUTest ../src lib)
target_link_libraries(${TEST_PROJECT} CppUTest Qt5::Core Qt5::Sql Threads::Threads)
