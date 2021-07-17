add_executable(test_qt_storage)

target_sources(test_qt_storage
PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/test_qt_storage_utils.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/QtStorageInitializer.cpp"
PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/QtStorageInitializer.h"
)

target_link_libraries(
    test_qt_storage
    PRIVATE
        sprint_timer_options
        sprint_timer_warnings
        gtest
        ${STORAGE_LIB}
)

gtest_discover_tests(test_qt_storage)
