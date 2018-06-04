set(TEST_CORE test_core)
project(${TEST_CORE})
set(SOURCE_FILES
        test_sprint_statistics.cpp
        test_distribution.cpp
        test_string_utils.cpp
        test_task.cpp
        test_stateful_timer.cpp
        test_core_service.cpp
        test_increment_task_sprints.cpp
        test_csv_reader.cpp
        test_csv_writer.cpp
        test_sprint_builder.cpp
        test_task_builder.cpp
        test_weekday_selection.cpp
        test_algutils.cpp
)

add_executable(${TEST_CORE} ${SOURCE_FILES})
target_compile_features(${TEST_CORE} PRIVATE cxx_std_17)

target_link_libraries(${TEST_CORE}
    PRIVATE gmock_main
    PRIVATE ${CORE_LIB}
)

# gtest_add_tests(TARGET ${TEST_CORE})
gtest_discover_tests(${TEST_CORE})