add_library(common_utils)

target_sources(common_utils
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include/common_utils/FakeUuidGenerator.h"
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/FakeUuidGenerator.cpp"
)

target_include_directories(common_utils PUBLIC include)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
if (MSVC)
	target_compile_definitions(common_utils PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

target_link_libraries(
    common_utils
    PUBLIC
        sprint_timer_options
        sprint_timer_warnings
        ${CORE_LIB}
        gmock_main
)
