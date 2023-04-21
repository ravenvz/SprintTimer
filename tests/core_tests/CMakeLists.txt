add_executable(test_core)

target_sources(test_core
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/mocks/WorkflowMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/test_algutils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_distribution.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_by_day.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_by_month.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_by_week.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_over_period.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_recurrence.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_sprint.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_stateful_timer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_string_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_tag_top.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_timeframe.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_tree.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_tree.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_work_schedule.cpp"
)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(test_core PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_link_libraries(
    test_core
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
        common_utils
        ${CORE_LIB}
        gmock_main
)

# gtest_add_tests(TARGET ${TEST_CORE})
gtest_discover_tests(test_core WORKING_DIRECTORY ${TEST_RUN_DIR})
