add_executable(test_core)

target_sources(test_core
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/mocks/ConfigMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/DataExporterMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/OperationalRangeReaderMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/QueryHandlerMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/SprintDistributionReaderMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/SprintStorageMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/TaskStorageMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/WorkflowMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/mocks/WorkScheduleStorageMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/test_add_new_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_algutils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_cancel_timer_command.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_change_work_schedule.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_delete_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_distribution.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_edit_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_export_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_export_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_by_day.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_by_month.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_by_week.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_over_period.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_register_new_sprint.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_register_new_sprint_bulk.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_remove_sprint.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_rename_tag.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_all_tags.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_finished_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_operational_range.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_sprint_distribution.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_sprints_for_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_unfinished_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_request_work_schedule.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_start_timer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_stateful_timer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_store_unfinished_tasks_order.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_string_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_toggle_zone_mode_command.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_toggle_task_completion_status.cpp"
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
