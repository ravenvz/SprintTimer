set(TEST_QT_STORAGE test_qt_storage)
project(${TEST_QT_STORAGE})

add_executable(${TEST_QT_STORAGE})

target_sources(${TEST_QT_STORAGE}
PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/test_qt_storage_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/test_storage_integration.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/QtStorageInitializer.cpp"
PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/QtStorageInitializer.h"
)

# target_include_directories(${TEST_QT_STORAGE}
#     PUBLIC ../include
#     PRIVATE ../src
# )

target_link_libraries(
    ${TEST_QT_STORAGE}
    PRIVATE
        project_options
        project_warnings
        gtest
        ${STORAGE_LIB}
)

gtest_discover_tests(${TEST_QT_STORAGE})
