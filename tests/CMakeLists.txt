set(PROJECT_TESTS sprint_timer_tests)
project(${PROJECT_TESTS})

# Download and unpack googletest at configure time
include(CMakeLists-gtest.txt)

# target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
# "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

add_subdirectory(core_tests)
add_subdirectory(qt_storage_impl_tests)
