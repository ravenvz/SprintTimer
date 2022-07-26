add_executable(test_api)

target_sources(test_api
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/CommandHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/FakeUuidGenerator.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/QtStorageInitializer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/QueryHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/TestCommandHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/TestQueryHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/fixtures/StatisticsDataFixture.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/matchers/MatchesSprintIgnoringUuid.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/matchers/MatchesTaskIgnoringUuid.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/mocks/DataExporterMock.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/api_tests/utils/utils.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/FakeUuidGenerator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/QtStorageInitializer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/TestCommandHandlerComposer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/TestQueryHandlerComposer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/fixtures/StatisticsDataFixture.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/matchers/MatchesSprintIgnoringUuid.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/matchers/MatchesTaskIgnoringUuid.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_changing_task_tree.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_changing_work_schedule.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_creating_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_editing_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_export_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_export_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_reading_sprint_distributions.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_registering_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_removing_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_removing_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_renaming_tag.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_reordering_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_active_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_daily_statistics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_finished_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_sprints_for_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_statistics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_tags.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_top_tag_frequencies.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_workday_statistics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_worktime_statistics.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_retrieving_operation_range.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_toggling_task_completion.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/utils/utils.cpp"
)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(test_api PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_include_directories(test_api PUBLIC include)

target_link_libraries(
    test_api
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
        common_utils
        ${STORAGE_LIB}
)

gtest_discover_tests(test_api WORKING_DIRECTORY ${TEST_RUN_DIR})
