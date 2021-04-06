set(TEST_PRESENTATION test_presentation)
project(${TEST_PRESENTATION})

add_executable(${TEST_PRESENTATION})

target_sources(${TEST_PRESENTATION}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/test_active_tasks_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_add_task_control_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_best_workday_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_daily_statistics_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_data_export_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_date_range_selector_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_daytime_statistics_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_history_mediator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_history_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_progress_view_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_register_sprint_control_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_sprint_outline_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_statistics_mediator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_tag_diagram_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_tag_editor_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_sprints_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_view_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_timer_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_today_progress_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_undo_presenter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_work_schedule_editor_presenter.cpp"
)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(${TEST_PRESENTATION} PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_link_libraries(
    ${TEST_PRESENTATION}
    PRIVATE
        project_options
        project_warnings
        gmock_main
        ${GUI_LIB}
)

# gtest_add_tests(TARGET ${TEST_PRESENTATION})
gtest_discover_tests(${TEST_PRESENTATION} WORKING_DIRECTORY ${TEST_RUN_DIR})
