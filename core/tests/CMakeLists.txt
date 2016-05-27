cmake_minimum_required(VERSION 3.2)

set(TEST_CORE test_pomodoro_core)
project(${TEST_CORE})
set(SOURCE_FILES
    run_all_tests.cpp
    test_datetime.cpp
    test_pomodoro_statistics.cpp
    test_distribution.cpp
    test_string_utils.cpp
    test_time_span.cpp
    test_todoitem.cpp
    test_pomodoro_timer.cpp
    test_pomodoro_service.cpp
    test_remove_task_transaction.cpp
    test_increment_spent_pomodoros.cpp
    ../src/DateTime.cpp
    ../src/PomodoroStatistics.cpp
    ../src/Pomodoro.cpp
    ../src/TodoItem.cpp
    ../src/TimeSpan.cpp
    ../src/StringUtils.cpp
    ../src/PomodoroTimer.cpp
    ../src/Timer.cpp
    ../src/PomodoroService.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TEST_CORE} ${SOURCE_FILES})
target_compile_features(${TEST_CORE} PRIVATE cxx_return_type_deduction)

# silence warnings from CppUTest
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    target_compile_options(${TEST_CORE} PRIVATE ${CLANG_COMPILE_OPTIONS}
                                                     -Wno-keyword-macro
                                                     -Wno-inconsistent-missing-override
                                                     -Wno-reserved-id-macro
                                                     -Wno-disabled-macro-expansion
                                                     -Wno-old-style-cast)
elseif(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${TEST_CORE} PRIVATE ${GCC_COMPILE_OPTIONS}
                                                   -Wno-unused-parameter)
else()
  message(FATAL_ERROR "Unknown compiler.")
endif()

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${TEST_CORE} 
    PRIVATE /usr/include/CppUTest
    PUBLIC ../include
    PRIVATE ../src
    PRIVATE fixtures
    PRIVATE lib)

target_link_libraries(${TEST_CORE} CppUTest Threads::Threads)
