add_executable(test_external_io)

target_sources(test_external_io
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/test_csv_reader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_csv_serialization_algorithm.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_runtime_data_exporter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_runtime_sink_router.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_serializer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_sprint_to_csv.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_to_csv.cpp"
)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(test_external_io PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_link_libraries(
    test_external_io
    PRIVATE
        project_options
        project_warnings
        gmock_main
        ${EXTERNAL_IO_LIB}
)

gtest_discover_tests(test_external_io WORKING_DIRECTORY ${TEST_RUN_DIR})
