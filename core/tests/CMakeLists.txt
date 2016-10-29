cmake_minimum_required(VERSION 3.2)

set(TEST_CORE test_core)
project(${TEST_CORE})
set(SOURCE_FILES
        run_all_tests.cpp
        test_datetime.cpp
        test_sprint_statistics.cpp
        test_distribution.cpp
        test_string_utils.cpp
        test_time_span.cpp
        test_task.cpp
        test_stateful_timer.cpp
        test_core_service.cpp
        test_increment_task_sprints.cpp
        test_csv_reader.cpp
        test_csv_writer.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${TEST_CORE} ${SOURCE_FILES})
target_compile_features(${TEST_CORE} PRIVATE cxx_return_type_deduction)

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

add_dependencies(${TEST_CORE} CppUTest)
target_include_directories(${TEST_CORE}
    PUBLIC ../include
    PRIVATE ../src
    PRIVATE fixtures)

target_link_libraries(${TEST_CORE}
    PRIVATE CppUTest
    PRIVATE ${CORE_LIB}
    PRIVATE Threads::Threads
)

# This is a workaround
# For some reason, although CppUTest explicitly depends on winmm, dependency does not transition.
if (WIN32)
    target_link_libraries(${TEST_CORE} PRIVATE winmm.lib)
endif()
