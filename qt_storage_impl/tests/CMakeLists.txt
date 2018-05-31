set(PROJECT test_qt_storage_impl)
project(${PROJECT})
set(SOURCE_FILES
        test_qt_storage_impl.cpp
)

add_executable(${PROJECT} ${SOURCE_FILES})
target_compile_features(${PROJECT} PRIVATE cxx_std_17)

target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>")
target_compile_options(${PROJECT} PRIVATE "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>")

target_include_directories(${PROJECT}
    PUBLIC ../include
    PRIVATE ../src
)

target_link_libraries(${PROJECT} gtest gtest_main gmock ${STORAGE_LIB} Threads::Threads)

# TODO check if this is still relevant with GTest on Win box
# This is a workaround
# For some reason, although CppUTest explicitly depends on winmm, dependency does not transition.
if (WIN32)
    target_link_libraries(${PROJECT} winmm.lib)
endif()
