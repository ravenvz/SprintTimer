set(TEST_QT_STORAGE test_qt_storage_impl)
project(${TEST_QT_STORAGE})

add_executable(${TEST_QT_STORAGE})

target_sources(${TEST_QT_STORAGE}
PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/test_qt_storage_impl.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_task_storage.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/FileDeleter.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/QtStorageInitializer.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

target_compile_features(${TEST_QT_STORAGE} PRIVATE cxx_std_17)
target_compile_options(${TEST_QT_STORAGE} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${TEST_QT_STORAGE} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${TEST_QT_STORAGE}
    PUBLIC ../include
    PRIVATE ../src
)

target_link_libraries(${TEST_QT_STORAGE} gtest ${STORAGE_LIB})

gtest_discover_tests(${TEST_QT_STORAGE})
