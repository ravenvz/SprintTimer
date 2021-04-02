set(TEST_EXTERNAL_IO test_external_io)
project(${TEST_EXTERNAL_IO})

add_executable(${TEST_EXTERNAL_IO})

target_sources(${TEST_EXTERNAL_IO}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/test_csv_reader.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_csv_serialization_algorithm.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_runtime_data_exporter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_runtime_sink_router.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_serializer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_sprint_to_csv.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_to_csv.cpp"
)

target_compile_features(${TEST_EXTERNAL_IO} PRIVATE cxx_std_20)
target_compile_options(${TEST_EXTERNAL_IO} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${TEST_EXTERNAL_IO} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(${TEST_EXTERNAL_IO} PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_link_libraries(${TEST_EXTERNAL_IO}
    PRIVATE gmock_main
    PRIVATE ${EXTERNAL_IO_LIB}
)

# gtest_add_tests(TARGET ${TEST_EXTERNAL_IO})
gtest_discover_tests(${TEST_EXTERNAL_IO} WORKING_DIRECTORY ${TEST_RUN_DIR})
