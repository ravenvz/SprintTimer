add_library(common_utils INTERFACE)

target_sources(common_utils
    INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}/include/common_utils/DateTimeProviderMock.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/common_utils/TaskDtoTreeFixtures.h"
)

target_include_directories(common_utils INTERFACE include)

# TODO Workaround. MSVC compiler cannot link gmock properly otherwise
# Check if it is still required when next version of gtest is released.
# if (MSVC)
# 	target_compile_definitions(common_utils PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
# endif()

target_link_libraries(
    common_utils
    INTERFACE
        sprint_timer_options
        sprint_timer_warnings
        ${CORE_LIB}
        gmock_main
)
