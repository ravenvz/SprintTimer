set(PROJECT test_qt_storage_impl)
project(${PROJECT})
set(SOURCE_FILES
        test_qt_storage_impl.cpp
)

add_executable(${PROJECT} ${SOURCE_FILES})
target_compile_features(${PROJECT} PRIVATE cxx_std_17)

target_include_directories(${PROJECT}
    PUBLIC ../include
    PRIVATE ../src
)

target_link_libraries(${PROJECT} gtest_main ${STORAGE_LIB})

gtest_discover_tests(${PROJECT})
