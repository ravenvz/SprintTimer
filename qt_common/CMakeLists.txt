cmake_minimum_required(VERSION 3.2)

set(PROJECT pomodoro_qt_common)
project(${PROJECT})

set(SOURCE_FILES
)

add_library(${PROJECT} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT} PUBLIC include)
target_compile_features(${PROJECT} PRIVATE cxx_return_type_deduction)
target_link_libraries(${PROJECT} Qt5::Core pomodoro_core)

# add_subdirectory(tests)
# target_link_libraries(test_qt_storage_impl)
