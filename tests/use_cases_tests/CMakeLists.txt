add_executable(test_use_cases)

target_sources(test_use_cases
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/CommandHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/QtStorageInitializer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/QueryHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/TestCommandHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/TestQueryHandlerComposer.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/matchers/MatchesSprintIgnoringUuid.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/use_cases_tests/utils/utils.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/QtStorageInitializer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/matchers/MatchesSprintIgnoringUuid.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/matchers/MatchesTaskIgnoringUuid.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/utils/utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_creating_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_editing_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_reading_sprint_distributions.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_registering_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_removing_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_removing_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_renaming_tag.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_reordering_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_active_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_finished_tasks.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_sprints.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_sprints_for_task.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_requesting_tags.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_retrieving_operation_range.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/test_toggling_task_completion.cpp"
)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(test_use_cases PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_include_directories(test_use_cases PUBLIC include)

target_link_libraries(
    test_use_cases
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
        ${CORE_LIB}
        ${STORAGE_LIB}
        gmock_main
)

gtest_discover_tests(test_use_cases WORKING_DIRECTORY ${TEST_RUN_DIR})
