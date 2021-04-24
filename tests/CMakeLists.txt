# Download and unpack googletest at configure time
include(CMakeLists-gtest.txt)

target_include_directories(gmock_main SYSTEM BEFORE INTERFACE
    "$<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${gmock_SOURCE_DIR}/include>")

add_subdirectory(core_tests)
add_subdirectory(qt_storage_tests)
add_subdirectory(presentation_tests)
add_subdirectory(external_io_tests)
add_subdirectory(use_cases_tests)
