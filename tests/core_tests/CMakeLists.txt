set(TEST_CORE test_core)
project(${TEST_CORE})

add_executable(${TEST_CORE})

target_sources(${TEST_CORE}
    PRIVATE
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
    "${CMAKE_CURRENT_LIST_DIR}/test_sprint_builder.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_sprint_statistics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_start_timer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_stateful_timer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_store_unfinished_tasks_order.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_string_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_builder.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_toggle_zone_mode_command.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_toggle_task_completion_status.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_work_schedule.cpp"
)

target_compile_features(${TEST_CORE} PRIVATE cxx_std_20)
target_compile_options(${TEST_CORE} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${TEST_CORE} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(${TEST_CORE} PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_link_libraries(${TEST_CORE}
    PRIVATE gmock_main
    PRIVATE ${CORE_LIB}
)

# gtest_add_tests(TARGET ${TEST_CORE})
gtest_discover_tests(${TEST_CORE} WORKING_DIRECTORY ${TEST_RUN_DIR})
